/*
 * alt_sys_init.c - HAL initialization source
 *
 * Machine generated for CPU 'CPU' in SOPC Builder design 'NIOS'
 * SOPC Builder design path: C:/Quartus/Embedded/ROVER/NIOS.sopcinfo
 *
 * Generated: Fri Sep 20 15:52:01 COT 2019
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"

#include <stddef.h>

/*
 * Device headers
 */

#include "altera_nios2_qsys_irq.h"
#include "altera_avalon_epcs_flash_controller.h"
#include "altera_avalon_jtag_uart.h"
#include "altera_avalon_sysid_qsys.h"
#include "altera_avalon_timer.h"
#include "altera_avalon_uart.h"
#include "altera_up_avalon_accelerometer_spi.h"
#include "altera_up_avalon_de0_nano_adc.h"

/*
 * Allocate the device storage
 */

ALTERA_NIOS2_QSYS_IRQ_INSTANCE ( CPU, CPU);
ALTERA_AVALON_EPCS_FLASH_CONTROLLER_INSTANCE ( EPCS, EPCS);
ALTERA_AVALON_JTAG_UART_INSTANCE ( JTAG, JTAG);
ALTERA_AVALON_SYSID_QSYS_INSTANCE ( SYS_ID, sys_id);
ALTERA_AVALON_TIMER_INSTANCE ( SYS_TIMER, sys_timer);
ALTERA_AVALON_UART_INSTANCE ( DIST1, DIST1);
ALTERA_AVALON_UART_INSTANCE ( DIST2, DIST2);
ALTERA_AVALON_UART_INSTANCE ( DIST3, DIST3);
ALTERA_AVALON_UART_INSTANCE ( DIST4, DIST4);
ALTERA_AVALON_UART_INSTANCE ( DIST5, DIST5);
ALTERA_AVALON_UART_INSTANCE ( DIST6, DIST6);
ALTERA_AVALON_UART_INSTANCE ( DIST7, DIST7);
ALTERA_AVALON_UART_INSTANCE ( DIST8, DIST8);
ALTERA_AVALON_UART_INSTANCE ( GPS, GPS);
ALTERA_AVALON_UART_INSTANCE ( XBEE, XBEE);
ALTERA_UP_AVALON_ACCELEROMETER_SPI_INSTANCE ( ACELEROMETRO_SPI, ACELEROMETRO_SPI);
ALTERA_UP_AVALON_DE0_NANO_ADC_INSTANCE ( ADC_DE0, ADC_DE0);

/*
 * Initialize the interrupt controller devices
 * and then enable interrupts in the CPU.
 * Called before alt_sys_init().
 * The "base" parameter is ignored and only
 * present for backwards-compatibility.
 */

void alt_irq_init ( const void* base )
{
    ALTERA_NIOS2_QSYS_IRQ_INIT ( CPU, CPU);
    alt_irq_cpu_enable_interrupts();
}

/*
 * Initialize the non-interrupt controller devices.
 * Called after alt_irq_init().
 */

void alt_sys_init( void )
{
    ALTERA_AVALON_TIMER_INIT ( SYS_TIMER, sys_timer);
    ALTERA_AVALON_EPCS_FLASH_CONTROLLER_INIT ( EPCS, EPCS);
    ALTERA_AVALON_JTAG_UART_INIT ( JTAG, JTAG);
    ALTERA_AVALON_SYSID_QSYS_INIT ( SYS_ID, sys_id);
    ALTERA_AVALON_UART_INIT ( DIST1, DIST1);
    ALTERA_AVALON_UART_INIT ( DIST2, DIST2);
    ALTERA_AVALON_UART_INIT ( DIST3, DIST3);
    ALTERA_AVALON_UART_INIT ( DIST4, DIST4);
    ALTERA_AVALON_UART_INIT ( DIST5, DIST5);
    ALTERA_AVALON_UART_INIT ( DIST6, DIST6);
    ALTERA_AVALON_UART_INIT ( DIST7, DIST7);
    ALTERA_AVALON_UART_INIT ( DIST8, DIST8);
    ALTERA_AVALON_UART_INIT ( GPS, GPS);
    ALTERA_AVALON_UART_INIT ( XBEE, XBEE);
    ALTERA_UP_AVALON_ACCELEROMETER_SPI_INIT ( ACELEROMETRO_SPI, ACELEROMETRO_SPI);
    ALTERA_UP_AVALON_DE0_NANO_ADC_INIT ( ADC_DE0, ADC_DE0);
}
