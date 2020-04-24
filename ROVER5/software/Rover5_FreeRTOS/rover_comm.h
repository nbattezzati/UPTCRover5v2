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



// FUNCTION: RoverSendMsg()
// Description: function to send a message over the RF channel to a specific recipient
// Parameters:  - to: recipient address
//              - str: string containing the message to send
//              - len: length of the string (messages longer than COMM_MAX_MSG_SIZE will be truncated)
// Return:      bytes sent if message is successfully queued, < 0 if failed
int32_t RoverSendMsg(uint8_t to, const char * str, uint16_t len);

// FUNCTION: RoverSendBroadcastMsg()
// Description: function to send a message over the RF channel to all listening recipients
// Parameters:  - str: string containing the message to send
//              - len: length of the string (messages longer than COMM_MAX_MSG_SIZE will be truncated)
// Return:      bytes sent if message is successfully queued, < 0 if failed
int32_t RoverSendBroadcastMsg(const char * str, uint16_t len);

// FUNCTION: RoverTaskComm()
// Description: main task of the communication module.
//              It contains the main loop that checks if new messages have arrived
// Parameters:  - pvParameters: pointer to task parameters (pass NULL, not used)
// Return:      -
void RoverTaskComm(void *pvParameters);



#endif /* ROVER_COMM_H_ */
