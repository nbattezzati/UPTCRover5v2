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
/*
typedef struct {
	uint16_t size;
	uint8_t buf[256];
} rover_msg_t;

void RoverSendMsg(rover_msg_t msg);
*/

// FUNCTION: TaskComm()
// Description: main task of the communication module.
//              It contains the main loop that checks if new messages have arrived
// Parameters:  - pvParameters: pointer to task parameters (pass NULL, not used)
// Return:      -
void RoverTaskComm(void *pvParameters);



#endif /* ROVER_COMM_H_ */
