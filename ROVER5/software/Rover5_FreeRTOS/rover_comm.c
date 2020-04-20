/*
 * rover_comm.c
 *
 *  Created on: 25/mar/2020
 *      Author: nbattezzati
 */

#include <stdlib.h>			// for NULL definition
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



#define MAX_QUEUE_LEN	(8)		// max number of messages in a queue



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


////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


// FUNCTION: String2Msg()
// Description: function to create a comm_msg_t from a string of characters
uint8_t String2Msg(const char * str, int8_t len, comm_msg_t * msg)
{
	uint8_t ret_val = 0;
	int i = 0;

	// check if the output message structure is not NULL
	if (msg != NULL) {
		// copy bytes from the string to the message buffer
		for (i=0; i<len && i<COMM_MAX_MSG_SIZE; ++i) {
			msg->buf[i] = str[i];
		}
		// set the last carriage return
		msg->buf[i] = '\r';
		// set message length
		msg->len = (len < COMM_MAX_MSG_SIZE ? len : COMM_MAX_MSG_SIZE) + 1;
		// set the return value
		ret_val = msg->len;
	}

	return ret_val;
}


// FUNCTION: RoverSendMsg()
// Description: function to send a message over the RF channel to a specific recipient
int8_t RoverSendMsg(uint8_t to, const comm_msg_t * msg)
{
	comm_msg_t tmp_msg;
	uint8_t ret_val = 0;

	// check if the queue exists
	if (tx_queue_ == NULL) {
		ret_val = COMM_ERR_OUT_OF_MEMORY;
	}
	else {
		// lock the mutex to be sure that nobody else is using the queue
		xSemaphoreTake(tx_queue_mutex_, portMAX_DELAY);
		// queue the message if the queue is not full
		if (xQueueSendToBack(tx_queue_, &tmp_msg, 0) == errQUEUE_FULL) {
			ret_val = COMM_ERR_QUEUE_FULL;
		}
		// release the mutex
		xSemaphoreGive(tx_queue_mutex_);
	}

	return ret_val;
}

// FUNCTION: RoverSendBroadcastMsg()
// Description: function to send a message over the RF channel to all listening recipients
int8_t RoverSendBroadcastMsg(const comm_msg_t * msg)
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


	// Initialize the last_wakeup_time variable with the current time.
	last_wakeup_time = xTaskGetTickCount();

	// initialize the internal resources
	tx_queue_mutex_ = xSemaphoreCreateMutex();
	tx_queue_ = xQueueCreate(MAX_QUEUE_LEN, sizeof(comm_msg_t));

	// open the XBEE UART device
	if ((uart_fd = open("/dev/XBEE",O_RDWR)) < 0) {
		fprintf(stderr, "[ERR] Failed to open the XBee serial port\n");
	}

	// start the task
	while(1)
	{
		// loop over the messages to be sent
		while (uxQueueMessagesWaiting(tx_queue_) > 0) {
			// get a message from the queue
			xSemaphoreTake(tx_queue_mutex_, portMAX_DELAY);
			xQueueReceive(tx_queue_, &tx_msg, 0);
			xSemaphoreGive(tx_queue_mutex_);

			// write the message to the UART
			printf("[DEBUG] writing message to UART\n");
			write(uart_fd, tx_msg.buf, tx_msg.len);

			// TODO: send the command to immediately transmit the message
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


