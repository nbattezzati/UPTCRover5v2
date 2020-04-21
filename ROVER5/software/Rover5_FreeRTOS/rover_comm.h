/*
 * rover_comm.h
 *
 * This file contains the software module for the Rover communication
 *
 *  Created on: 25/mar/2020
 *      Author: nbattezzati
 */

#ifndef ROVER_COMM_H_
#define ROVER_COMM_H_


#define COMM_ERR_OUT_OF_MEMORY		(-1)
#define COMM_ERR_QUEUE_FULL			(-2)


#define COMM_MAX_MSG_SIZE			(255)	// max number of bytes in a message



typedef struct {
	uint8_t len;
	uint8_t *buf;
} comm_msg_t;

// FUNCTION: String2Msg()
// Description: function to create a comm_msg_t from a string of characters
// Parameters:  - str: string containing the message to send
//              - len: length of the string (max=COMM_MAX_MSG_SIZE chars. Characters in excess will be truncated)
//              - msg: pointer to a comm_msg_t that will be filled with the message
// Return:      number of characters copied into the message. If any error occurs return 0
uint8_t String2Msg(const char * str, uint8_t len, comm_msg_t * msg);

// FUNCTION: RoverSendMsg()
// Description: function to send a message over the RF channel to a specific recipient
// Parameters:  - to: recipient address
//              - msg: the message to be sent
// Return:      0 on successful queuing of the message, < 0 if failed
int8_t RoverSendMsg(uint8_t to, const comm_msg_t * msg);

// FUNCTION: RoverSendBroadcastMsg()
// Description: function to send a message over the RF channel to all listening recipients
// Parameters:  - msg: the message to be sent
// Return:      0 on successful queuing of the message, < 0 if failed
int8_t RoverSendBroadcastMsg(const comm_msg_t * msg);

// FUNCTION: RoverTaskComm()
// Description: main task of the communication module.
//              It contains the main loop that checks if new messages have arrived
// Parameters:  - pvParameters: pointer to task parameters (pass NULL, not used)
// Return:      -
void RoverTaskComm(void *pvParameters);



#endif /* ROVER_COMM_H_ */
