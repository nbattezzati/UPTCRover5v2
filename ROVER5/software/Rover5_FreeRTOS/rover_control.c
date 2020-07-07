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
#include "rover_comm.h"

#include "rover_pose_controller.h"

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

/* Constants defined by the mobile robot model */
#define R_WHEEL_ONLY		30						//radius of the wheel in mm
#define R_TRACKED			117.5					//radius of the belt in mm
#define N					333.3333				//number of ticks per revolution in the encoder 1000/3
#define L_WHEEL_SEPARATION	190						//wheel separation in mm
#define MM_PER_TICK			(2*M_PI*R_WHEEL_ONLY)/N	//mm traveled each tick

/* Controller parameters */
#define PID_KP  2.0f
#define PID_KI  0.5f
#define PID_KD  0.25f

#define PID_TAU 0.01f

#define PID_LIM_MIN -10.0f
#define PID_LIM_MAX  10.0f

#define SAMPLE_TIME_S 0.01f

/* Print Status Options */
#define TELEMETRY_VALUES	0x00
#define ODOMETRY_VALUES		0x01

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

/* FPGA variables */
unsigned char led_val = 0;
unsigned char sw_val = 0;

////////////////////////////
///                      ///
///    PRIVATE TYPES     ///
///                      ///
////////////////////////////

typedef struct {
	int32_t right;
	int32_t left;
} rover_encoders_t;

typedef struct {
	int32_t r1;
	int32_t r2;
	int32_t l1;
	int32_t l2;
} rover_encoders_raw_t;

typedef struct {
	float right;
	float left;
} rover_wheel_velocities_t;

typedef enum{
	RoverDirection_Forward 		= 0x35,
	RoverDirection_Right 		= 0x36,
	RoverDirection_Left 		= 0x39,
	RoverDirection_Backwards 	= 0x3A
} rover_direction_t;

////////////////////////////
///                      ///
/// FUNCTION PROTOTYPES  ///
///                      ///
////////////////////////////

void SetMobileRobotVelocity(rover_velocities_t * velocities, rover_wheel_velocities_t * wheel_velocities, rover_direction_t * direction);
void UpdateOdometry(rover_pose_t * pose, rover_encoders_t * delta_enc, rover_encoders_raw_t * enc_raw);
static void irqkey (void * context, alt_u32 id);
void PrintStatus (uint8_t opt_to_print, rover_pose_t * pose, rover_pose_t * pose_error, rover_encoders_raw_t * enc, rover_velocities_t * velocities, rover_wheel_velocities_t * wheel_velocities);

////////////////////////////
///                      ///
///   PUBLIC FUNCTIONS   ///
///                      ///
////////////////////////////


// FUNCTION: TaskControl()
// Description: main task of the control module.
void RoverTaskControl(void *pvParameters)
{
	uint8_t msg_period = 0;
	uint8_t move_period = 0;

	TickType_t last_wakeup_time = 0;

	// Initialize the last_wakeup_time variable with the current time.
	last_wakeup_time = xTaskGetTickCount();

	rover_pose_control_params_t params;

	// command reception variables
	comm_receiver_t cmd_receiver	= NULL;
	char * command_str				= NULL;

	//printf("Robot ID = 0 - ADSG 2\n");

	/* Set the initial robot position (x, y, theta) */
	rover_pose_t pose = {0, 0, 0};

	/* Set the robot goal pose (x_d, y_d, theta_d) */
	rover_pose_t desired_pose = {1, 1, M_PI};

	/* rover pose error (output from the controller) */
	rover_pose_t pose_error;

	/* Initialise PID controller */
	rover_PID_const_t pid = { PID_KP, PID_KI, -PID_KD, PID_TAU, PID_LIM_MIN, PID_LIM_MAX, SAMPLE_TIME_S };
	params.pid = &pid;
	PoseController_Init(RoverPoseControl_PID, &params);

	/* Initialise Lyapunov controller */
	rover_lyapunov_const_t lyapunov = {1, 1, 1, 1};
	params.lyapunov = &lyapunov;
	PoseController_Init(RoverPoseControl_Lyapunov, &params);
	/* Set the constants for the Lyapunov Control function (K1, K2, Q1, Q2) */
	//rover_lyapunov_const_t lyapunov_const = {0.5, 0.5, 0, 0.5};

	/* Initialize the direction of the robots motion */
	rover_direction_t direction = RoverDirection_Forward;

	/* Initialize the linear and angular velocities */
	rover_velocities_t velocities = {0};

	/* Initialize the wheel velocities */
	rover_wheel_velocities_t wheel_velocities = {0};

	/* Create a variable that holds the cumulative variation of the encoders averaged on the two wheels */
	rover_encoders_t delta_enc = {0};
	/* ... and also the raw values for each encoder */
	rover_encoders_raw_t enc_raw = {0};

	/* Register the FPGA Key interrupt service routine */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0x3);				//KEY FPGA
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0x0);
	alt_irq_register( KEY_IRQ, NULL,(void*)irqkey );


	// Register listener for commands
	cmd_receiver = RoverRegisterMsgReceiver();


	while(1){

		//printf("T_control RUNNING\n");

		// check if there is a new command
		if (RoverGetMsg(cmd_receiver, &command_str) > 0) {
			printf("New COMMAND received: %s\n", command_str);
			RoverReleaseMsg(command_str);
		}

		UpdateOdometry(&pose, &delta_enc, &enc_raw);

		/* Set the desired control function */
		//PoseController_Run(RoverPoseControl_Lyapunov, &params, &pose, &desired_pose, &pose_error, &velocities);

		//SetMobileRobotVelocity(&velocities, &wheel_velocities, &direction);

		//IOWR(MOTORES_BASE,0,RoverDirection_Forward);
		//IOWR(PWM1_BASE,0,255);
		//IOWR(PWM2_BASE,0,255);

		/* Wait for 10 control periods to send a message */
		if (msg_period == 10){
			PrintStatus(ODOMETRY_VALUES, &pose, &pose_error, &enc_raw, &velocities, &wheel_velocities);
			msg_period = 0;
		}
		++msg_period;

		/* Wait for the next cycle. */
		vTaskDelayUntil( &last_wakeup_time, TASK_MS_2_TICKS(RoverTaskControl_PERIOD));

	}

}

////////////////////////////
///                      ///
///   PRIVATE FUNCTIONS  ///
///                      ///
////////////////////////////

void SetMobileRobotVelocity(rover_velocities_t * velocities, rover_wheel_velocities_t * wheel_velocities, rover_direction_t * direction){

	wheel_velocities->left = 1*(((2*velocities->v)-(velocities->w*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY));
	wheel_velocities->right = 1*(((2*velocities->w)+(velocities->w*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY));

	if (wheel_velocities->left >= 0 && wheel_velocities->right >= 0){
		*direction = RoverDirection_Forward;
	} else if (wheel_velocities->left <= 0 && wheel_velocities->right <= 0){
		wheel_velocities->left = fabs(wheel_velocities->left);
		wheel_velocities->right = fabs(wheel_velocities->right);
		*direction = RoverDirection_Backwards;
	} else if (wheel_velocities->left >= 0 && wheel_velocities->right <= 0){
		wheel_velocities->right = fabs(wheel_velocities->right);
		*direction = RoverDirection_Right;
	} else if (wheel_velocities->left <= 0 && wheel_velocities->right >= 0){
		wheel_velocities->left = fabs(wheel_velocities->left);
		*direction = RoverDirection_Left;
	} else {
		*direction = RoverDirection_Forward;
	}

	if(wheel_velocities->right > 255){
		wheel_velocities->right = 255;
	//}else if (right_wheel_velocity_normal < 50){
	//	right_wheel_velocity_normal = 0;
	}
	if(wheel_velocities->left > 255){
		wheel_velocities->left = 255;
	//}else if (left_wheel_velocity_normal < 50){
	//	left_wheel_velocity_normal = 0;
	}

	IOWR(MOTORES_BASE,0,*direction);
	IOWR(PWM1_BASE,0,wheel_velocities->right);
	IOWR(PWM2_BASE,0,wheel_velocities->left);
}

void UpdateOdometry(rover_pose_t * pose, rover_encoders_t * delta_enc, rover_encoders_raw_t * enc_raw)
{
	rover_pose_t pose_dt = {0};

	/* Odometry variables */
	float Dr=0;
	float Dl=0;
	float Dc=0;

	volatile uint32_t encoder_ip = IORD(ENCODER_READING_BASE,0);
	enc_raw->r2 = ((encoder_ip>>0)  & 0x000000FF);
	enc_raw->r1 = ((encoder_ip>>8)  & 0x000000FF);
	enc_raw->l2  = ((encoder_ip>>16) & 0x000000FF);
	enc_raw->l1  = ((encoder_ip>>24) & 0x000000FF);

	//printf("Encoder ip value: 0x%.8x \n", encoder_ip);
	//printf("Encoder values: l1 %d, l2 %d, r1 %d r2 %d \n", cur_left_enc1, cur_left_enc2, cur_right_enc1, cur_right_enc2);

	/*
	 *	Update the cumulative encoders
	 */
	delta_enc->right	= enc_raw->r2;
	delta_enc->left 	= - enc_raw->l2;

	/*
	 *	Compute Dl, Dr and Dc
	 */
	Dr = MM_PER_TICK * delta_enc->right;
	Dl = MM_PER_TICK * delta_enc->left;
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

}

/*************************************************************************
**************************************************************************
**																		**
** Function:    PrintStatus()											**
**																		**
** Description: Funciï¿½n donde se envía por consola el estatus de  	**
** 				diferentes variables del robot							**
** 																		**
** Notes:       En esta funciï¿½n se define un mensaje a imprimir		**
** 				en consola de acuerdo con las variables que se deseen 	**
** 				observar.												**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
void PrintStatus (
	uint8_t opt_to_print,
	rover_pose_t * pose,
	rover_pose_t * pose_error,
	rover_encoders_raw_t * enc,
	rover_velocities_t * velocities,
	rover_wheel_velocities_t * wheel_velocities)
{
  if (opt_to_print == TELEMETRY_VALUES){
    RoverSendMsg_MOTOR_TELEM(1,0,
    	wheel_velocities->left, wheel_velocities->right,
    	0, 0, /* TODO: get motor currents */
    	enc->l1, enc->l2, enc->r1, enc->r2);
  }
  else if (opt_to_print == ODOMETRY_VALUES){
    RoverSendMsg_CONTROL_OUTPUT(1,0,
        pose->x, pose->y, pose->theta,
        velocities->v,velocities->w,
        pose_error->x, pose_error->y, pose_error->theta);
  }
  else {
     printf("[ERR] Unknown print option (%d)!\n", opt_to_print);
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
** Description: Funciï¿½n que define la interrupciï¿½n de los pulsadores**
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
