/*
 * rover_control.c
 *
 *  Created on: 2/04/2020
 *      Author: Andres Suarez
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "rover_task.h"

#include <math.h> 			// for the math functions
#include <stdint.h>			// for the uint types
#include <stdbool.h>		// for the bool types

/* System specific includes. */
#include "io.h"										// for reading or writing to I/O
#include "system.h"									// for the configuration and register of the QSYS system
#include "sys/alt_irq.h"
#include "altera_up_avalon_de0_nano_adc.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"


////////////////////////////
///                      ///
///       DEFINES        ///
///                      ///
////////////////////////////

/* Mobile robot movement directions */
#define BACKWARDS		0x3A
#define FORWARD			0x35
#define LEFT		 	0x39
#define RIGHT		 	0x36

/* Constants defined by the mobile robot model */
#define R_WHEEL_ONLY		30						//radius of the wheel in mm
#define R_TRACKED			117.5					//radius of the belt in mm
#define N					83.3333					//number of ticks per revolution in the encoder 250/3
#define L_WHEEL_SEPARATION	155						//wheel separation in mm
#define MM_PER_TICK			(2*M_PI*R_WHEEL_ONLY)/N	//mm traveled each tick

/* Print Status Options */
#define ENCODER_VALUES		0x00
#define ODOMETRY_VALUES		0x01
#define VELOCITY_VALUES		0x02

/* Macro for the encoder ticks average*/
#define ENCODER_AVERAGE(ENC_VALUE_1, ENC_VALUE_2) ((ENC_VALUE_1+ENC_VALUE_2)/2)


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

/* Encoder variables */
static volatile int8_t left_encoder_1	= 0;
static volatile int8_t left_encoder_2	= 0;
static volatile int8_t right_encoder_1	= 0;
static volatile int8_t right_encoder_2	= 0;



/* Mobile robot direction */
unsigned char sentido_giro = 0;



/* Control variables */
float set_linear_velocity=0, theta_desired = 0;
float Kp_theta=0, Ki_theta=0, Kd_theta = 0;
float error_theta = 0;
float e_dot = 0;
float e_old = 0;
float E=0;
float u=0;

/* Robot velocites */
float linear_velocity=0, angular_velocity = 0;

/* FPGA variables */
unsigned char led_val = 0;
unsigned char sw_val = 0;

////////////////////////////
///                      ///
///    PRIVATE TYPES     ///
///                      ///
////////////////////////////

/* Mobile robot pose */
typedef struct {
	float x;
	float y;
	float theta;
} rover_pose_t;

typedef struct {
	int32_t right;
	int32_t left;
} rover_encoders_t;


////////////////////////////
///                      ///
/// FUNCTION PROTOTYPES  ///
///                      ///
////////////////////////////

void SetMobileRobotVelocity();
void UpdateOdometry(rover_pose_t * pose, rover_encoders_t * enc);
float PIDControl (float error, float Kp, float Ki, float Kd);
static void irqencoders (void * context, alt_u32 id);
static void irqkey (void * context, alt_u32 id);
void PrintStatus (uint8_t opt_to_print, const rover_pose_t * pose, const rover_encoders_t * enc);

////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


// FUNCTION: TaskControl()
// Description: main task of the control module.
void RoverTaskControl(void *pvParameters)
{
	TickType_t last_wakeup_time = 0;

	// Initialize the last_wakeup_time variable with the current time.
	last_wakeup_time = xTaskGetTickCount();

	//printf("Robot ID = 0 - ADSG 2\n");

	/* Initialize odometry and control variables*/
	//encoder1=0, encoder2=0, encoder3=0, encoder4=0;
	//right_ticks = 0, left_ticks = 0, prev_right_ticks = 0, prev_left_ticks = 0;
	//right_encoder_1 = 0, right_encoder_2 = 0, left_encoder_1 = 0, left_encoder_2 = 0;
	//Dr=0, Dl=0,	Dc=0, e_dot = 0, e_old = 0,	E=0, u=0;

	/* Set the initial robot position (x, y, theta) */
	rover_pose_t pose = {0, 0, 0};

	// create a varibale that holds the cumulative variation of the encoders
	// averaged on the two wheels
	rover_encoders_t cumulative_enc = {0};

	/* Define the required variables for control */
	//theta_desired = - M_PI_2;
	//set_linear_velocity = 5.5;
	//Kp_theta = 20, Ki_theta = 0, Kd_theta = 0;

	/* Register the encoder interrupt service routine */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(ENCODER_INT_BASE, 0xF);		//ENCODERS FPGA -- ENABLES
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE, 0x0);
	alt_irq_register( ENCODER_INT_IRQ, NULL,(void*)irqencoders );

	/* Register the FPGA Key interrupt service routine */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0x3);				//KEY FPGA
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0x0);
	alt_irq_register( KEY_IRQ, NULL,(void*)irqkey );

	while(1){
		//sprintf(msg, "%d", (int)left_encoder_1);

		printf("T_control RUNNING\n");



		UpdateOdometry(&pose, &cumulative_enc);

		//error_theta = theta_desired - theta;
		//error_theta = atan2f(sinf(error_theta), cosf(error_theta));

		//angular_velocity = PIDControl(error_theta, Kp_theta, Ki_theta, Kd_theta);
		//linear_velocity = set_linear_velocity;

		//SetMobileRobotVelocity();

		//PrintStatus(ODOMETRY_VALUES);

		// It does not work on its own, only works by connecting the FPGA to the PC
		if (pose.x < 100){
			IOWR(MOTORES_BASE,0,FORWARD);
			IOWR(PWM2_BASE,0,255);
			IOWR(PWM1_BASE,0,255);
		} else {
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);

		}
		//By changing the type of data and reseting the variables after Update Odometry it fixes the problem
		/*
		if (left_encoder_1 < 100 || left_encoder_2 < 100 || right_encoder_1 < 100 ||right_encoder_2 < 100){
			IOWR(MOTORES_BASE,0,FORWARD);
			IOWR(PWM2_BASE,0,255);
			IOWR(PWM1_BASE,0,255);
		} else {
			IOWR(MOTORES_BASE,0,0);
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);
		}
		*/
		// Wait for the next cycle.
		vTaskDelayUntil( &last_wakeup_time, TASK_MS_2_TICKS(RoverTaskControl_PERIOD));
	}

}

////////////////////////////
///                      ///
///   PRIVATE FUNCTIONS  ///
///                      ///
////////////////////////////

void SetMobileRobotVelocity(){

	float left_wheel_velocity = 0, right_wheel_velocity = 0;
	float left_wheel_velocity_normal = 0, right_wheel_velocity_normal = 0;

	left_wheel_velocity = ((2*linear_velocity)-(angular_velocity*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY);
	right_wheel_velocity = ((2*linear_velocity)+(angular_velocity*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY);

	if (left_wheel_velocity >= 0 && right_wheel_velocity >= 0){
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = FORWARD;
	} else if (left_wheel_velocity <= 0 && right_wheel_velocity <= 0){
		left_wheel_velocity_normal = fabs(left_wheel_velocity);
		right_wheel_velocity_normal = fabs(right_wheel_velocity);
		sentido_giro = BACKWARDS;
	} else if (left_wheel_velocity >= 0 && right_wheel_velocity <= 0){
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = fabs(right_wheel_velocity);
		sentido_giro = RIGHT;
	} else if (left_wheel_velocity <= 0 && right_wheel_velocity >= 0){
		left_wheel_velocity_normal = fabs(left_wheel_velocity);
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = LEFT;
	} else {
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = FORWARD;
	}

	if(right_wheel_velocity_normal > 255){
		right_wheel_velocity_normal = 255;
	//}else if (right_wheel_velocity_normal < 50){
	//	right_wheel_velocity_normal = 0;
	}
	if(left_wheel_velocity_normal > 255){
		left_wheel_velocity_normal = 255;
	//}else if (left_wheel_velocity_normal < 50){
	//	left_wheel_velocity_normal = 0;
	}


}

void UpdateOdometry(rover_pose_t * pose, rover_encoders_t * cumulative_enc)
{
	rover_pose_t pose_dt = {0};

	/* Odometry variables */
	float Dr=0;
	float Dl=0;
	float Dc=0;

	// copy and immediately reset the variables shared with ISR.
	// In the manner if an interrupt comes between the copy and the reset instructions
	// we're likely to loose only 1 tick.
	// If we reset them at the end we loose all the ticks that arrived
	// during the execution of the function.
	taskENTER_CRITICAL();
	int8_t cur_right_enc1 = right_encoder_1;
	right_encoder_1 = 0;
	int8_t cur_right_enc2 = right_encoder_2;
	right_encoder_2 = 0;
	int8_t cur_left_enc1 = left_encoder_1;
	left_encoder_1 = 0;
	int8_t cur_left_enc2 = left_encoder_2;
	left_encoder_2 = 0;
	taskEXIT_CRITICAL();

	/*
	 *	Update the cumulative encoders
	 */
	cumulative_enc->right	+= ENCODER_AVERAGE(cur_right_enc1, cur_right_enc2);
	cumulative_enc->left	+= ENCODER_AVERAGE(cur_left_enc1, cur_left_enc2);

	/*
	 *	Compute Dl, Dr and Dc
	 */
	Dr = MM_PER_TICK * ENCODER_AVERAGE(cur_right_enc1, cur_right_enc2);
	Dl = MM_PER_TICK * ENCODER_AVERAGE(cur_left_enc1, cur_left_enc2);
	Dc = ((Dr+Dl)/2);

	/*
	 * Compute the x_dt, y_dt, theta_dt values from Dr, Dl, Dc
	 */
	pose_dt.x = Dc*cosf(pose->theta);
	pose_dt.y = Dc*sinf(pose->theta);
	pose_dt.theta = ((Dr-Dl)/L_WHEEL_SEPARATION);

	/*
	* Compute the new values x_new, y_new, theta_new for the robot pose
	*/
	pose->theta += pose_dt.theta;
	pose->x += pose_dt.x;
	pose->y += pose_dt.y;

	/*
	 * Set to 0 the encoder values
	 */
	left_encoder_1 = 0;
	left_encoder_2 = 0;
	right_encoder_1 = 0;
	right_encoder_2 = 0;
}

float PIDControl (float e, float Kp, float Ki, float Kd){

	e_dot = e - e_old;
	E=E+e;
	u = (Kp*e) + (Kd*e_dot) + (Ki*E);
	e_old = e;

	return u;
}



/*************************************************************************
**************************************************************************
**																		**
** Function:    PrintStatus()											**
**																		**
** Description: Funci�n donde se env�a por consola el estatus de  	**
** 				diferentes variables del robot							**
** 																		**
** Notes:       En esta funci�n se define un mensaje a imprimir		**
** 				en consola de acuerdo con las variables que se deseen 	**
** 				observar.												**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
void PrintStatus (uint8_t opt_to_print, const rover_pose_t * pose, const rover_encoders_t * enc)
{
	if (opt_to_print == ENCODER_VALUES){
		printf("rigthEnc: %d - leftEnc: %d ", (int)enc->right, (int)enc->left);
	}
	else if (opt_to_print == ODOMETRY_VALUES){
		printf("Error theta: %f \n ",error_theta);
		printf("x: %f \n ",pose->x);
		printf("y: %f \n ",pose->y);
		printf("theta: %f \n ",pose->theta);
	}
	else if (opt_to_print == VELOCITY_VALUES){
		printf("Linear velocity: %f \n ",linear_velocity);
		printf("Angular velocity: %f \n ",angular_velocity);
	}
	else {
		printf("Not specified values!");
	}
}

/*
 * Interrupt Functions
 */

/*************************************************************************
**************************************************************************
**																		**
** Function:    irqkey()												**
**																		**
** Description: Funci�n que define la interrupci�n de los pulsadores**
** 				de la tarjeta DE0 Nano									**
** 																		**
** Notes:       N/A														**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void irqkey (void * context, alt_u32 id){
	led_val = 0;
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0x3);
	IOWR_16DIRECT(KEY_BASE, 0, 0); // reset request
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    irqencoders()											**
**																		**
** Description: Funci�n que define la interrupci�n de los encoders de**
** 				la plataforma ROVER 5, la versi�n que se usa tiene 	**
** 				cuatro encoders de cuadratura							**
** 																		**
** Notes:       Al tener cuatro encoders de cuadratura se puede 		**
** 				determinar la direcci�n de giro de las llantas		**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void irqencoders (void * context, alt_u32 id){

	unsigned char entrada;

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x1;

	/*********************************************************************
	** Multiplica el valor por 0001 para eliminar los valores de los	**
	** dem�s encoders, luego determina el sentido y aumenta encoder1,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x1) != 0){
			left_encoder_1--;
		}
		else{
			left_encoder_1++;
		}
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x2;

	/*********************************************************************
	** Multiplica el valor por 0010 para eliminar los valores de los	**
	** dem�s encoders, luego determina el sentido y aumenta encoder2,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x2) != 0){
			left_encoder_2++;
		}
		else{
			left_encoder_2--;
		}
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x4;

	/*********************************************************************
	** Multiplica el valor por 0100 para eliminar los valores de los	**
	** dem�s encoders, luego determina el sentido y aumenta encoder3,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x4) != 0){
			right_encoder_1--;
		}
		else{
			right_encoder_1++;
		}
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x8;

	/*********************************************************************
	** Multiplica el valor por 1000 para eliminar los valores de los	**
	** dem�s encoders, luego determina el sentido y aumenta encoder4,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x8) != 0){
			right_encoder_2--;
		}
		else{
			right_encoder_2++;
		}
	}

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(ENCODER_INT_BASE, 0xf);
	IOWR_16DIRECT(ENCODER_INT_BASE, 0, 0); // reset request
}
