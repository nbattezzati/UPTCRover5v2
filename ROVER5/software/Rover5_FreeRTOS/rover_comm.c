/*
 * rover_comm.c
 *
 *  Created on: 25/mar/2020
 *      Author: nbattezzati
 */

#include <stdio.h>
#include <unistd.h> 		// for the function usleep

#include "FreeRTOS.h"
#include "task.h"

#include "rover_task.h"

#include "io.h"										// for reading or writing to I/O
#include "system.h"									// for the configuration and register of the QSYS system
#include "sys/alt_irq.h"
#include "altera_avalon_uart.h"
#include "altera_avalon_uart_regs.h"

////////////////////////////
///                      ///
///   PUBLIC VARIABLES   ///
///                      ///
////////////////////////////


////////////////////////////
///                      ///
///   PRIVATE VARIABLES  ///
///                      ///
////////////////////////////

/* Xbee variables */
unsigned char bufferxbee[1000]={0};
int punteroxbee = 0;
unsigned char entradaxbee=0;

/* Predefined messages */
char* listo = "O0:U0:ROBOT LISTO \r \n";
char* recibido = "O0:U0:COMANDO RECIBIDO \r \n";
char* fallido = "O0:U0:COMANDO DESCONOCIDO \r \n";
char* sensores = "O0:U0:ENC XXX XXX XXX XXX XXX XXX XXX XXX \r \n";

////////////////////////////
///                      ///
///    PRIVATE TYPES     ///
///                      ///
////////////////////////////



////////////////////////////
///                      ///
/// FUNCTION PROTOTYPES  ///
///                      ///
////////////////////////////
void mensaje_xbee(char *mensaje);
static void isrxbee (void * context, alt_u32 id);

////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


// FUNCTION: TaskComm()
// Description: main task of the communication module.
void RoverTaskComm(void *pvParameters)
{
	TickType_t last_wakeup_time = 0;

	// Initialize the last_wakeup_time variable with the current time.
	last_wakeup_time = xTaskGetTickCount();

	int context_xbee;
	alt_irq_register(XBEE_IRQ,&context_xbee,isrxbee); // install XBEE ISR
	alt_irq_enable (XBEE_IRQ);

	while(1)
	{
		printf("T_comm RUNNING\n");

		// Wait for the next cycle.
		vTaskDelayUntil( &last_wakeup_time, TASK_MS_2_TICKS(1000) );
	}
}



////////////////////////////
///                      ///
///   PRIVATE FUNCTIONS  ///
///                      ///
////////////////////////////

/*************************************************************************
**************************************************************************
**																		**
** Function:    mensaje_xbee()											**
**																		**
** Description: Funci�n donde hace el env�o de un mensaje por medio de 	**
** 				la tarjeta XBee											**
** 																		**
** Notes:       En esta funci�n se define un char *mensaje, en donde	**
** 				mensaje es un apuntador que recorre los caracteres del 	**
** 				mensaje que se quiere enviar.							**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
void mensaje_xbee(char *mensaje){

	while(*mensaje){
		IOWR_ALTERA_AVALON_UART_TXDATA(XBEE_BASE, *mensaje);
		mensaje++;
		usleep(1200); // Sleep de 2mS
	}
}

/*
 * Interrupt Functions
 */

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrxbee()												**
**																		**
** Description: Funci�n que lee lo que llega a la tarjeta XBee y llena  **
** 				un buffer con estos datos. ISR							**
** 																		**
** Notes:       Cuando el buffer llega a 500 o se presiona ENTER, se 	**
** 				llama la funci�n an�lisis() para determinar los 		**
** 				comandos recibidos.										**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrxbee (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(XBEE_BASE);
	//IOWR_ALTERA_AVALON_UART_TXDATA(XBEE_BASE, test);
	entradaxbee = test;

	/**************************************************************************
	** Determina se si presiona un ENTER o el buffer tiene un valor superior **
	** a 500 posiciones de memoria, analiza el buffer y reinicia el puntero  **
	**************************************************************************/
	if (test == 13 || punteroxbee == 500){ // En ASCII 13 equivale a ENTER
		//analisis();
		//analiso lo que me llego !!
		punteroxbee = 0;
	}
	/**********************************************************************
	** Llena el buffer de acuerdo a los valores que se est�n recibiendo  **
	** en la tarjeta XBee												 **
	**********************************************************************/
	else{
		bufferxbee[punteroxbee] = test;
		punteroxbee++;
	}
}
