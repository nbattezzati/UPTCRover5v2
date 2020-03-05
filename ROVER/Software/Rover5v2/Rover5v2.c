/*********************************************************************************************
**********************************************************************************************
**																							**
**  Autor		:	Andrés David Suárez Gómez												**
**  Tutor		:	Wilson Javier Pérez Holguín												**
**  Institución	: 	Universidad Pedagógica y Tecnológica de Colombia (UPTC)					**
**  Entidad		:	Grupo de Investigacion en Robótica y Automatización Industrial (GIRA)	**
**  Descripción :	Código para el procesador NIOS II que comanda a un robot desarrollado	**
**  				por el grupo de investigación GIRA con aplicación a robótica educativa	**
**  				y Sistemas Multi-Robot.													**
**  																						**
**********************************************************************************************
*********************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "io.h"
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include <sys/alt_irq.h> // the irq functions
#include <alt_types.h>
#include "altera_avalon_timer_regs.h"
#include "altera_up_avalon_de0_nano_adc.h"
#include "altera_avalon_uart_regs.h"
#include "altera_avalon_uart.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include <math.h>
//#include <altera_up_avalon_accelerometer_spi_spi.h>
//#include "altera_up_avalon_accelerometer.h"

#define TRUE 1
#define FALSE 0

/******************************
** Direcciones de movimiento **
** del robot				 **
******************************/
#define ATRAS 		0x3A
#define ADELANTE	0x35
#define IZQUIERDA 	0x39
#define DERECHA 	0x36

//#define PI			3.14159
//M_PI = P, M_TWOPI = 2*PI, M_PI_2 = PI/2, M_PI_4 = PI/4, M_3PI_4 = 3*PI/4, M_SQRTPI = RAIZ(PI), M_1_PI = 1/PI, M_2_PI = 2/PI

#define R_WHEEL_ONLY		0.03					//radius of the wheel in m
#define R_TRACKED			0.1175					//radius of the belt in m
#define N					83.3333					//number of ticks per revolution in the encoder 250/3
#define L_WHEEL_SEPARATION	0.155					//wheel separation in m
#define M_PER_TICK			(2*M_PI*R_WHEEL_ONLY)/N	//meters traveled each tick

float x = 0,y = 0,theta = 0;
float x_dt = 0,y_dt = 0,theta_dt = 0;
float x_new = 0,y_new = 0,theta_new = 0;

signed long right_ticks=0, left_ticks=0, prev_right_ticks=0, prev_left_ticks = 0;

float set_linear_velocity=0, theta_desired = 0;

float linear_velocity=0, angular_velocity = 0;

float left_wheel_velocity = 0;
float right_wheel_velocity = 0;
float left_wheel_velocity_normal = 0;
float right_wheel_velocity_normal = 0;
unsigned char sentido_giro = 0;

float Dr=0,Dl=0,Dc=0;
float e_dot = 0;
float e_old = 0;
float E=0;
float u=0;

float error_theta = 0;

float Kp_theta=0, Ki_theta=0, Kd_theta = 0;

/*****************
** ID del robot **
*****************/
#define robotid 	'0'

/*******************************
** Valores de LEDs y switches **
*******************************/
unsigned char led_val = 0;
unsigned char sw_val = 0;

/*****************************
** Mensajes del Robot al PC **
*****************************/
char* listo = "O0:U0:ROBOT LISTO \r \n";
char* recibido = "O0:U0:COMANDO RECIBIDO \r \n";
char* fallido = "O0:U0:COMANDO DESCONOCIDO \r \n";
char* sensores = "O0:U0:ENC XXX XXX XXX XXX XXX XXX XXX XXX \r \n";

/*****************************
** Control Iniciar Comandos **
*****************************/
char	arranque = 0;
char	circulo = 0;
char 	circsentido = 0;
int     circpwm = 0;
unsigned long circdist = 0;

/**************************
** Buffers UART para los **
** sensores ultrasónicos **
**************************/
unsigned char buffer1[5];
unsigned char buffer2[5];
unsigned char buffer3[5];
unsigned char buffer4[5];
unsigned char buffer5[5];
unsigned char buffer6[5];
unsigned char buffer7[5];
unsigned char buffer8[5];

/*************************
** Buffers UART para la **
** tarjeta XBee         **
*************************/

unsigned char bufferxbee[1000];

/**************************
** Apuntadores para los  **
** sensores ultrasónicos **
**************************/
unsigned char puntero1 = 0;
unsigned char puntero2 = 0;
unsigned char puntero3 = 0;
unsigned char puntero4 = 0;
unsigned char puntero5 = 0;
unsigned char puntero6 = 0;
unsigned char puntero7 = 0;
unsigned char puntero8 = 0;

/*************************
** Apuntadores para la  **
** tarjeta XBee         **
*************************/
int punteroxbee = 0;

/**********************************************************************
** Variables relacionadas con XBee, sensores de distancia y encoders **
**********************************************************************/
unsigned int sensordist1=0, sensordist2=0, sensordist3=0, sensordist4 = 0, sensordist5=0, sensordist6=0, sensordist7=0, sensordist8 = 0;
long encoder1=0, encoder2=0, encoder3=0, encoder4 = 0;
unsigned char sentido_encoder1, sentido_encoder2, sentido_encoder3, sentido_encoder4;
unsigned char entradaxbee;

signed long left_encoder_1=0, left_encoder_2=0, right_encoder_1=0, right_encoder_2 = 0;

/**************
** Funciones **
**************/
void interrupciones();

void SetMobileRobotVelocity();
void UpdateOdometry();
long GetRightTicks(long ticks_right_encoder_1, long ticks_right_encoder_2);
long GetLeftTicks(long ticks_left_encoder_1, long ticks_left_encoder_2);
float PIDControl (float error, float Kp, float Ki, float Kd);

void mensaje_xbee(char *mensaje);

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrxbee()												**
**																		**
** Description: Función que lee lo que llega a la tarjeta XBee y llena  **
** 				un buffer con estos datos. ISR							**
** 																		**
** Notes:       Cuando el buffer llega a 500 o se presiona ENTER, se 	**
** 				llama la función análisis() para determinar los 		**
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
	** Llena el buffer de acuerdo a los valores que se están recibiendo  **
	** en la tarjeta XBee												 **
	**********************************************************************/
	else{
		bufferxbee[punteroxbee] = test;
		punteroxbee++;
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist1()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 1 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist1 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST1_BASE);

	if (test == 'R'){
		puntero1 = 0;
		buffer1[puntero1] = test;
		puntero1++;
	}
	else{
		buffer1[puntero1] = test - 48;
		puntero1++;
	}

	if (test == 13){
		sensordist1 = (buffer1[1]*1000 + buffer1[2]*100 + buffer1[3]*10 + buffer1[4]); //ASCII  a binario
		//sensordist1 = (buffer1[1]*100 + buffer1[2]*10 + buffer1[3]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist2()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 2 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist2 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST2_BASE);

	if (test == 'R'){
		puntero2 = 0;
		buffer2[puntero2] = test;
		puntero2++;
	}
	else{
		buffer2[puntero2] = test - 48;
		puntero2++;
	}

	if (test == 13){
		sensordist2 = (buffer2[1]*1000 + buffer2[2]*100 + buffer2[3]*10 + buffer2[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist3()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 3 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist3 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST3_BASE);

	if (test == 'R'){
		puntero3 = 0;
		buffer3[puntero3] = test;
		puntero3++;
	}
	else{
		buffer3[puntero3] = test - 48;
		puntero3++;
	}

	if (test == 13){
		sensordist3 = (buffer3[1]*1000 + buffer3[2]*100 + buffer3[3]*10 + buffer3[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist4()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 4 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist4 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST4_BASE);

	if (test == 'R'){
		puntero4 = 0;
		buffer4[puntero4] = test;
		puntero4++;
	}
	else{
		buffer4[puntero4] = test - 48;
		puntero4++;
	}

	if (test == 13){
		sensordist4 = (buffer4[1]*1000 + buffer4[2]*100 + buffer4[3]*10 + buffer4[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist5()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 5 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist5 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST5_BASE);

	if (test == 'R'){
		puntero5 = 0;
		buffer5[puntero5] = test;
		puntero5++;
	}
	else{
		buffer5[puntero5] = test - 48;
		puntero5++;
	}

	if (test == 13){
		sensordist5 = (buffer5[1]*1000 + buffer5[2]*100 + buffer5[3]*10 + buffer5[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist6()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 6 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist6 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST6_BASE);

	if (test == 'R'){
		puntero6 = 0;
		buffer6[puntero6] = test;
		puntero6++;
	}
	else{
		buffer6[puntero6] = test - 48;
		puntero6++;
	}

	if (test == 13){
		sensordist6 = (buffer6[1]*1000 + buffer6[2]*100 + buffer6[3]*10 + buffer6[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist7()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 7 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist7 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST7_BASE);

	if (test == 'R'){
		puntero7 = 0;
		buffer7[puntero7] = test;
		puntero7++;
	}
	else{
		buffer7[puntero7] = test - 48;
		puntero7++;
	}

	if (test == 13){
		sensordist7 = (buffer7[1]*1000 + buffer7[2]*100 + buffer7[3]*10 + buffer7[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    isrdist8()												**
**																		**
** Description: Función que lee los valores de distancia del sensor 	**
** 				ultrasonico 8 usando UART								**
** 																		**
** Notes:       La comunicación del sensor es RS232, la salida es un 	**
** 				ASCII 'R', seguida de tres ASCII que representan el		**
** 				valor de la distancia en pulgadas y termina con un 		**
** 				ASCII 13 (CR o Enter). Baud=9600, 8 bits, sin paridad	**
** 				con un bit de parada									**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
static void isrdist8 (void * context, alt_u32 id){

	unsigned char test;
	test = IORD_ALTERA_AVALON_UART_RXDATA(DIST8_BASE);

	if (test == 'R'){
		puntero8 = 0;
		buffer8[puntero8] = test;
		puntero8++;
	}
	else{
		buffer8[puntero8] = test - 48;
		puntero8++;
	}

	if (test == 13){
		sensordist8 = (buffer8[1]*1000 + buffer8[2]*100 + buffer8[3]*10 + buffer8[4]);
	}
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    irqkey()												**
**																		**
** Description: Función que define la interrupción de los pulsadores 	**
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
** Description: Función que define la interrupción de los encoders de 	**
** 				la plataforma ROVER 5, la versión que se usa tiene 		**
** 				cuatro encoders de cuadratura							**
** 																		**
** Notes:       Al tener cuatro encoders de cuadratura se puede 		**
** 				determinar la dirección de giro de las llantas			**
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
	** demás encoders, luego determina el sentido y aumenta encoder1,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x1) != 0){
			sentido_encoder1 = 0;
			left_encoder_1--; //para la plataforma 2
			// left_encoder_1++; //para la plataforma 1
		}
		else{
			sentido_encoder1 = 1;
			left_encoder_1++; //para la plataforma 2
			// left_encoder_1--; //para la plataforma 1
		}
		encoder1++;
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x2;

	/*********************************************************************
	** Multiplica el valor por 0010 para eliminar los valores de los	**
	** demás encoders, luego determina el sentido y aumenta encoder2,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x2) != 0){
			sentido_encoder2 = 0;
			//left_encoder_2++;
			left_encoder_2--; //para la plataforma 3
		}
		else{
			sentido_encoder2 = 1;
			//left_encoder_2--;
			left_encoder_2++; //para la plataforma 3
		}
		encoder2++;
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x4;

	/*********************************************************************
	** Multiplica el valor por 0100 para eliminar los valores de los	**
	** demás encoders, luego determina el sentido y aumenta encoder3,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x4) != 0){
			sentido_encoder3 = 0;
			right_encoder_1--;
		}
		else{
			sentido_encoder3 = 1;
			right_encoder_1++;
		}
		encoder3++;
	}

	entrada = IORD_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE) & 0x8;

	/*********************************************************************
	** Multiplica el valor por 1000 para eliminar los valores de los	**
	** demás encoders, luego determina el sentido y aumenta encoder4,	**
	** esta valiable que cuenta el numero de pulsos que se obtienen del	**
	** sensor para medir la distancia recorrida por la llanta			**
	*********************************************************************/
	if (entrada != 0){
		if ((IORD(ENCODER_NORMAL_BASE,0) & 0x8) != 0){
			sentido_encoder4 = 0;
			//right_encoder_2--; //para la plataforma 2
			right_encoder_2++; //para la plataforma 1 y 3
		}
		else{
			sentido_encoder4 = 1;
			//right_encoder_2++; // para la plataforma 2
			right_encoder_2--; // para la plataforma 1 y 3
		}
		encoder4++;
	}

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE, 0);
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(ENCODER_INT_BASE, 0xf);
	IOWR_16DIRECT(ENCODER_INT_BASE, 0, 0); // reset request
}

/*************************************************************************
**************************************************************************
**																		**
** Function:    main()													**
**																		**
** Description: Función principal del robot móvil						**
** 																		**
** Notes:       En esta función se hacen los llamados a las demas 		**
**              funciones del sistema de acuerdo a las tareas del robot	**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
int main(){

	mensaje_xbee(listo);
	usleep(10000); // Sleep de 10ms
	interrupciones();		// arranca las interrupciones

	printf("Robot ID = 0 - ADSG 2\n");
	entradaxbee = ' ';

	Dr=0;
	Dl=0;
	Dc=0;
	e_dot = 0;
	e_old = 0;
	E=0;
	u=0;

	//corra(ADELANTE,100);

	//set_linear_velocity = 5;
	set_linear_velocity = 5.5;
	//theta_desired = M_PI+0.5;
	//theta_desired = (M_PI)+ M_PI/2;
	theta_desired = - M_PI_2;
	x = 0;
	y = 0;
	theta = 0;
	Kp_theta = 20;
	Ki_theta = 0;
	//Kp_theta = 0.5;  //valores correctos para moverse a -PI/2, aunque es un poco lento el movimiento
	//Ki_theta = 0.005;
	Kd_theta = 0;
	encoder1=0;
	encoder2=0;
	encoder3=0;
	encoder4=0;

	right_ticks = 0;
	left_ticks = 0;
	right_encoder_1 = 0;
	right_encoder_2 = 0;
	left_encoder_1 = 0;
	left_encoder_2 = 0;
	prev_right_ticks = 0;
	prev_left_ticks = 0;
	usleep(1000000);

	while(1){

		right_ticks = GetRightTicks(right_encoder_1,right_encoder_2);
		left_ticks = GetLeftTicks(left_encoder_1,left_encoder_2);

		UpdateOdometry();

		//error_theta = theta_desired - theta;
		//error_theta = atan2f(sinf(error_theta), cosf(error_theta));

		//angular_velocity = PIDControl(error_theta, Kp_theta, Ki_theta, Kd_theta);
		//linear_velocity = set_linear_velocity;

		//SetMobileRobotVelocity();
		/*
		printf("Valor velocidad lineal: %f \n ",linear_velocity);
		printf("Valor velocidad angular: %f \n ",angular_velocity);
		printf("Valor velocidad rueda derecha normal: %f \n ",right_wheel_velocity_normal);
		printf("Valor velocidad rueda izquierda normal: %f \n ",left_wheel_velocity_normal);
		printf("Valor velocidad rueda derecha: %f \n ",right_wheel_velocity);
		printf("Valor velocidad rueda izquierda: %f \n ",left_wheel_velocity);
		printf("Sentido de giro: %d \n", sentido_giro );
		printf("Error theta: %f \n ",error_theta);
		printf("x: %f \n ",x);
		printf("y: %f \n ",y);
		printf("theta: %f \n ",theta);
		*/
		/*
		if ((fabs(error_theta)) <= 0.2){
			IOWR(MOTORES_BASE,0,ADELANTE);
			IOWR(PWM2_BASE,0,200);
			IOWR(PWM1_BASE,0,200);
		}else if (error_theta <= 0){
			IOWR(MOTORES_BASE,0,DERECHA);
			IOWR(PWM2_BASE,0,200);
			IOWR(PWM1_BASE,0,200);
		}else if (error_theta > 0){
			IOWR(MOTORES_BASE,0,IZQUIERDA);
			IOWR(PWM2_BASE,0,200);
			IOWR(PWM1_BASE,0,200);
		}
		*/
		/*
		printf("e: %f x: %f y: %f theta: %f ", error_theta, x, y, theta);
		printf("re1: %ld re2: %ld ", right_encoder_1,right_encoder_2);
		printf("le1: %ld le2: %ld \r \n", left_encoder_1,left_encoder_2);
		*/

		//IOWR(MOTORES_BASE,0,sentido_giro);
		//IOWR(PWM2_BASE,0,((int)(left_wheel_velocity_normal)));
		//IOWR(PWM1_BASE,0,((int)(right_wheel_velocity_normal)));
		/*********************************************************
		** Manejo del robot por medio de XBee, al presionar la	**
		** tecla correspondiente el robot se mueve en alguna 	**
		** dirección específica indefinidamente					**
		*********************************************************/
		/*if (entradaxbee == 'w'){
			IOWR(MOTORES_BASE,0,ADELANTE);
			IOWR(PWM2_BASE,0,220);
			IOWR(PWM1_BASE,0,200);
			//mensaje_xbee(recibido);
		}
		if (entradaxbee == 's'){
			IOWR(MOTORES_BASE,0,ATRAS);
			IOWR(PWM2_BASE,0,255);
			IOWR(PWM1_BASE,0,255);
			//mensaje_xbee(recibido);
		}
		if (entradaxbee == 'a'){
			IOWR(MOTORES_BASE,0,IZQUIERDA);
			IOWR(PWM2_BASE,0,255);
			IOWR(PWM1_BASE,0,255);
			mensaje_xbee(recibido);
		}
		if (entradaxbee == 'd'){
			IOWR(MOTORES_BASE,0,DERECHA);
			IOWR(PWM2_BASE,0,255);
			IOWR(PWM1_BASE,0,255);
			//mensaje_xbee(recibido);
		}
		if (entradaxbee == ' '){
			IOWR(MOTORES_BASE,0,0);					// quieto
			IOWR(PWM1_BASE,0,0);
			IOWR(PWM2_BASE,0,0);
			//mensaje_xbee(recibido);
		}
		*/
		/*********************************************************
		** Prueba de control on/off para desplazamiento del 	**
		** robot en una distancia determinada				 	**
		*********************************************************/
		/*if (y >= 1){
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);
		} else {
			IOWR(MOTORES_BASE,0,ADELANTE);
			IOWR(PWM2_BASE,0,200);
			IOWR(PWM1_BASE,0,150);

		}
		*/
		//IOWR(MOTORES_BASE,0,ADELANTE);
		//IOWR(PWM2_BASE,0,200);
		//IOWR(PWM1_BASE,0,200);

		/*if (x <= -4){
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);
		} else {
			IOWR(MOTORES_BASE,0,ATRAS);
			IOWR(PWM2_BASE,0,170);
			IOWR(PWM1_BASE,0,255);
		}*/
		//IOWR(LED_BASE,0,255);
		//IOWR(MOTORES_BASE,0,ADELANTE);
		//IOWR(PWM2_BASE,0,170);
		//IOWR(PWM1_BASE,0,255);

		IOWR(MOTORES_BASE,0,ADELANTE);
		IOWR(PWM2_BASE,0,255);
		IOWR(PWM1_BASE,0,255);

		/*************************************************
		** Prueba de lectura de valores de los sensores	**
		** ultrasonicos y los encoders del robot		**
		**************************************************/
		/*if(sensordist1<=310){
			IOWR(MOTORES_BASE,0,0);
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);
			mensaje_xbee(recibido);
		}
		*/
		/*
		if (left_encoder_1 >= 250 || left_encoder_2 >= 250 || right_encoder_1 >= 250 || right_encoder_2 >= 250){
			IOWR(MOTORES_BASE,0,0);
			IOWR(PWM2_BASE,0,0);
			IOWR(PWM1_BASE,0,0);
		}
		*/
		/*
		printf("Valor encoders derecha 1: %ld \n ",right_encoder_1);
		printf("Valor encoders derecha 2: %ld \n ",right_encoder_2);
		printf("Valor encoders izquierda 1: %ld \n ",left_encoder_1);
		printf("Valor encoders izquierda 2: %ld \n ",left_encoder_2);
		*/
		/*printf("right ticks %ld \n ",right_ticks);
		printf("left ticks %ld \n ",left_ticks);
		printf("x: %f \n ",x);
		printf("y: %f \n ",y);
		printf("theta: %f \n ", theta);*/
	}
	return 0;
}

void SetMobileRobotVelocity(){

	left_wheel_velocity = ((2*linear_velocity)-(angular_velocity*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY);
	right_wheel_velocity = ((2*linear_velocity)+(angular_velocity*L_WHEEL_SEPARATION))/(2*R_WHEEL_ONLY);

	if (left_wheel_velocity >= 0 && right_wheel_velocity >= 0){
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = ADELANTE;
	} else if (left_wheel_velocity <= 0 && right_wheel_velocity <= 0){
		left_wheel_velocity_normal = fabs(left_wheel_velocity);
		right_wheel_velocity_normal = fabs(right_wheel_velocity);
		sentido_giro = ATRAS;
	} else if (left_wheel_velocity >= 0 && right_wheel_velocity <= 0){
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = fabs(right_wheel_velocity);
		sentido_giro = DERECHA;
	} else if (left_wheel_velocity <= 0 && right_wheel_velocity >= 0){
		left_wheel_velocity_normal = fabs(left_wheel_velocity);
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = IZQUIERDA;
	} else {
		left_wheel_velocity_normal = left_wheel_velocity;
		right_wheel_velocity_normal = right_wheel_velocity;
		sentido_giro = ADELANTE;
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

long GetRightTicks(long ticks_right_encoder_1, long ticks_right_encoder_2){
	return (ticks_right_encoder_1+ticks_right_encoder_2)/2;
}
long GetLeftTicks(long ticks_left_encoder_1, long ticks_left_encoder_2){
	return (ticks_left_encoder_1+ticks_left_encoder_2)/2;
}

void UpdateOdometry(){
	/*
	 *	Compute Dl, Dr and Dc
	 */

	Dr=M_PER_TICK*(right_ticks-prev_right_ticks);
	Dl=M_PER_TICK*(left_ticks-prev_left_ticks);
	Dc=((Dr+Dl)/2);

	/*
	 * Compute the x_dt, y_dt, theta_dt values from Dr, Dl, Dc
	 */
	x_dt = Dc*cosf(theta);
	y_dt = Dc*sinf(theta);
	theta_dt = ((Dr-Dl)/L_WHEEL_SEPARATION);

	/*
	* Compute the new values x_new, y_new, theta_new for the robot pose
	*/
	theta_new = theta + theta_dt;
	x_new = x + x_dt;
	y_new = y + y_dt;

	/*
	 * Save the encoder ticks for the next estimate
	 */
	prev_right_ticks = right_ticks;
	prev_left_ticks = left_ticks;

	/*
	 * Update the pose estimation of the robot
	 */
	x = x_new;
	y = y_new;
	theta = theta_new;
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
** Function:    interrupciones()										**
**																		**
** Description: Función donde se definen las interrupciones 			**
** 																		**
** Notes:       En esta función se definen interrupciones asociadas con **
** 				los encoders, la tarjeta XBee, el GPS y los sensores de **
** 				distancia												**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
void interrupciones() {

	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(KEY_BASE, 0x3);				//KEY FPGA
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(KEY_BASE, 0x0);
	alt_irq_register( KEY_IRQ, NULL,(void*)irqkey );

	encoder1 = 0;
	encoder2 = 0;
	encoder3 = 0;
	encoder4 = 0;
	left_encoder_1 = 0;
	left_encoder_2 = 0;
	right_encoder_1 = 0;
	right_encoder_2 = 0;
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(ENCODER_INT_BASE, 0xF);		//ENCODERS FPGA -- ENABLES
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(ENCODER_INT_BASE, 0x0);
	alt_irq_register( ENCODER_INT_IRQ, NULL,(void*)irqencoders );

	int context_xbee;
	alt_irq_register(XBEE_IRQ,&context_xbee,isrxbee); // install XBEE ISR
	alt_irq_enable (XBEE_IRQ);

	int context_dist1;
	alt_irq_register(DIST1_IRQ,&context_dist1,isrdist1); // install DISTANCIA 1 ISR
	alt_irq_enable (DIST1_IRQ);

	int context_dist2;
	alt_irq_register(DIST2_IRQ,&context_dist2,isrdist2); // install DISTANCIA 2 ISR
	alt_irq_enable (DIST2_IRQ);

	int context_dist3;
	alt_irq_register(DIST3_IRQ,&context_dist3,isrdist3); // install DISTANCIA 3 ISR
	alt_irq_enable (DIST3_IRQ);

	int context_dist4;
	alt_irq_register(DIST4_IRQ,&context_dist4,isrdist4); // install DISTANCIA 4 ISR
	alt_irq_enable (DIST4_IRQ);

	int context_dist5;
	alt_irq_register(DIST5_IRQ,&context_dist5,isrdist5); // install DISTANCIA 5 ISR
	alt_irq_enable (DIST5_IRQ);

	int context_dist6;
	alt_irq_register(DIST6_IRQ,&context_dist6,isrdist6); // install DISTANCIA 6 ISR
	alt_irq_enable (DIST6_IRQ);

	int context_dist7;
	alt_irq_register(DIST7_IRQ,&context_dist7,isrdist7); // install DISTANCIA 3 ISR
	alt_irq_enable (DIST7_IRQ);

	int context_dist8;
	alt_irq_register(DIST8_IRQ,&context_dist8,isrdist8); // install DISTANCIA 4 ISR
	alt_irq_enable (DIST8_IRQ);

}

/*************************************************************************
**************************************************************************
**																		**
** Function:    mensaje_xbee()											**
**																		**
** Description: Función donde hace el envío de un mensaje por medio de 	**
** 				la tarjeta XBee											**
** 																		**
** Notes:       En esta función se define un char *mensaje, en donde	**
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

/*************************************************************************
**************************************************************************
**																		**
** Function:    moveForward()											**
**																		**
** Description: Función para mover el robot en línea recta hacia	 	**
** 				adelante a una velocidad constante						**
** 																		**
** Notes:       En esta función se se define un char *mensaje, en donde	**
** 				mensaje es un apuntador que recorre los caracteres del 	**
** 				mensaje que se quiere enviar.							**
**																		**
** Returns:     N/A														**
**																		**
**************************************************************************
*************************************************************************/
void moveForward(float distance, float velocity){
	
}
