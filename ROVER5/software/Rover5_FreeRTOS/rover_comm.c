/*
 * rover_comm.c
 *
 *  Created on: 25/mar/2020
 *      Author: nbattezzati
 */

#include <stdlib.h>			// for NULL definition
#include <string.h>			// for memcpy()
#include <stdbool.h>		// for bool type
#include <sys/types.h>		// for open() syscall
#include <sys/stat.h>		// for open() syscall
#include <fcntl.h>			// for open() syscall
#include <unistd.h>			// for write() syscall
#include <stdio.h>			// for fprintf()

#include <FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include <task.h>

#include "rover_task.h"
#include "rover_comm.h"

#include "altera_avalon_uart.h"


#define COMM_MSG_TRAILING_STR			"\0\r"	// every message ends with this string
#define COMM_MSG_TRAILING_STR_SIZE		(2)		// size of the string that closes a message
#define COMM_MAX_MSG_SIZE				(1024)	// max number of bytes in a message

#define MAX_QUEUE_LEN					(8)		// max number of messages in a queue





////////////////////////////
///                      ///
///   PUBLIC VARIABLES   ///
///     (to avoid!)      ///
////////////////////////////


////////////////////////////
///                      ///
///    PRIVATE TYPES     ///
///                      ///
////////////////////////////

// TYPE: comm_msg_t
// Description: structure used to send a message over a queue
// Fields:      - len: length of the message
//              - buf: pointer to the buffer containing the message (dynamically allocated)
typedef struct {
	uint16_t	len;
	uint8_t *	buf;
} comm_msg_t;


////////////////////////////
///                      ///
///   PRIVATE VARIABLES  ///
///                      ///
////////////////////////////

SemaphoreHandle_t tx_queue_mutex_ = NULL;
QueueHandle_t tx_queue_ = NULL;



////////////////////////////
///                      ///
/// FUNCTION PROTOTYPES  ///
///                      ///
////////////////////////////

// FUNCTION: string_2_msg()
// Description: function to create a comm_msg_t from a string of characters (allocating the memory for the message)
uint16_t string_2_msg(const char * str, uint16_t len, comm_msg_t * msg);


////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


// FUNCTION: RoverSendMsg()
// Description: function to send a message over the RF channel to a specific recipient
int32_t RoverSendMsg(uint8_t to, const char * str, uint16_t len)
{
	uint8_t ret_val = 0;
	comm_msg_t msg = {0};
	uint16_t target_len = 0;

	// check if the queue exists
	if (tx_queue_ != NULL) {
		// check if the length is ok, otherwise truncate the message
		target_len = (len < COMM_MAX_MSG_SIZE ? len : COMM_MAX_MSG_SIZE) + COMM_MSG_TRAILING_STR_SIZE;
		// create the message structure
		if (string_2_msg(str, target_len, &msg) > 0) {
			// lock the mutex to be sure that nobody else is using the queue
			xSemaphoreTake(tx_queue_mutex_, portMAX_DELAY);
			// queue the message if the queue is not full
			printf("[DEBUG] sending msg to queue: len=%d, msg=%s\n", msg.len, msg.buf);
			if (xQueueSend(tx_queue_, &msg, 0) != pdTRUE) {
				printf("[ERR] Failed to send message to queue\n");
				free(msg.buf);
				ret_val = COMM_ERR_QUEUE_FULL;
			}
			// release the mutex
			xSemaphoreGive(tx_queue_mutex_);
		}
		else if (len > 0) {
			ret_val = COMM_ERR_OUT_OF_MEMORY;
		}
		else {
			// len=0, do nothing
		}
	}
	else {
		printf("[ERR] Cannot send message to a NULL queue\n");
		ret_val = COMM_ERR_OUT_OF_MEMORY;
	}

	return ret_val;
}

// FUNCTION: RoverSendBroadcastMsg()
// Description: function to send a message over the RF channel to all listening recipients
int32_t RoverSendBroadcastMsg(const char * str, uint16_t len)
{
	// TODO RoverSendBroadcastMsg()
	return 0;
}


// FUNCTION: TaskComm()
// Description: main task of the communication module.
void RoverTaskComm(void *pvParameters)
{
	TickType_t last_wakeup_time = 0;
	int uart_fd = 0;
	comm_msg_t tx_msg;
	bool msg_valid = true;


	// Initialize the last_wakeup_time variable with the current time.
	last_wakeup_time = xTaskGetTickCount();

	// initialize the internal resources
	if ((tx_queue_mutex_ = xSemaphoreCreateMutex()) == NULL) {
		printf("[ERR] failed to create TX queue Mutex\n");
	}
	else {
		if ((tx_queue_ = xQueueCreate(MAX_QUEUE_LEN, sizeof(comm_msg_t))) == NULL) {
			printf("[ERR] failed to create TX queue\n");
		}
		else {
			// open the XBEE UART device
			if ((uart_fd = open("/dev/XBEE",O_RDWR)) < 0) {
				fprintf(stderr, "[ERR] Failed to open the XBee serial port\n");
			}
		}
	}

	// start the task
	while(1)
	{
		// loop over the messages to be sent
		while ((tx_queue_ != NULL) && (uxQueueMessagesWaiting(tx_queue_) > 0)) {
			msg_valid = true;
			// get a message from the queue
			xSemaphoreTake(tx_queue_mutex_, portMAX_DELAY);
			if (xQueueReceive(tx_queue_, &tx_msg, 0) == pdTRUE) {
				msg_valid = false;
				printf("[ERR] Failed to receive msg from TX queue\n");
			}
			xSemaphoreGive(tx_queue_mutex_);

			// write the message to the UART
			if (msg_valid == true) {
				printf("[DEBUG] writing message to UART: len=%d, msg=%s\n", tx_msg.len, tx_msg.buf);
				write(uart_fd, tx_msg.buf, tx_msg.len);
				// release the memory for the buffer holding the message
				free(tx_msg.buf);

				// TODO: send the command to immediately transmit the message
			}
		}

		// Wait for the next cycle.
		vTaskDelayUntil( &last_wakeup_time, TASK_MS_2_TICKS(1) );
	}
}



////////////////////////////
///                      ///
///   PRIVATE FUNCTIONS  ///
///                      ///
////////////////////////////

// FUNCTION: String2Msg()
// Description: function to create a comm_msg_t from a string of characters (allocating the memory for the message)
// Parameters:  - str: string containing the message
//              - len: length of the message
//              - msg: pointer to a comm_msg_t that will be filled with the message
// Return:      number of characters copied into the message. If any error occurs return 0
uint16_t string_2_msg(const char * str, uint16_t len, comm_msg_t * msg)
{
	uint16_t ret_val = 0;

	// allocate the buffer
	if ((msg != NULL) && (len > 0) && ((msg->buf = malloc(len)) != NULL)) {
		// copy bytes from the string to the message buffer
		memcpy(msg->buf, str, len-COMM_MSG_TRAILING_STR_SIZE);
		// set the last character to end the string
		memcpy(msg->buf+len-COMM_MSG_TRAILING_STR_SIZE, COMM_MSG_TRAILING_STR, COMM_MSG_TRAILING_STR_SIZE);
		// set message length
		msg->len = len;
		// set return value
		ret_val = msg->len;

		printf("[DEBUG] string_2_msg: len=%d, msg=%s\n", msg->len, msg->buf);
	}
	else {
		printf("[ERR] failed to allocate memory for msg buffer (or msg is NULL)\n");
	}

	return ret_val;
}
