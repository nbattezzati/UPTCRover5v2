/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'CPU' in SOPC Builder design 'NIOS'
 * SOPC Builder design path: C:/Quartus/Embedded/ROVER/NIOS.sopcinfo
 *
 * Generated: Tue Sep 24 13:01:15 COT 2019
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

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * ACELEROMETRO_SPI configuration
 *
 */

#define ACELEROMETRO_SPI_BASE 0x4002260
#define ACELEROMETRO_SPI_IRQ 5
#define ACELEROMETRO_SPI_IRQ_INTERRUPT_CONTROLLER_ID 0
#define ACELEROMETRO_SPI_NAME "/dev/ACELEROMETRO_SPI"
#define ACELEROMETRO_SPI_SPAN 2
#define ACELEROMETRO_SPI_TYPE "altera_up_avalon_accelerometer_spi"
#define ALT_MODULE_CLASS_ACELEROMETRO_SPI altera_up_avalon_accelerometer_spi


/*
 * ADC_DE0 configuration
 *
 */

#define ADC_DE0_BASE 0x4002140
#define ADC_DE0_IRQ -1
#define ADC_DE0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ADC_DE0_NAME "/dev/ADC_DE0"
#define ADC_DE0_SPAN 32
#define ADC_DE0_TYPE "altera_up_avalon_de0_nano_adc"
#define ALT_MODULE_CLASS_ADC_DE0 altera_up_avalon_de0_nano_adc


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_qsys"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x04001820
#define ALT_CPU_CPU_FREQ 100000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "small"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1b
#define ALT_CPU_DCACHE_LINE_SIZE 0
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 0
#define ALT_CPU_DCACHE_SIZE 0
#define ALT_CPU_EXCEPTION_ADDR 0x02000020
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 100000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INST_ADDR_WIDTH 0x1b
#define ALT_CPU_NAME "CPU"
#define ALT_CPU_RESET_ADDR 0x04001000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x04001820
#define NIOS2_CPU_FREQ 100000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "small"
#define NIOS2_DATA_ADDR_WIDTH 0x1b
#define NIOS2_DCACHE_LINE_SIZE 0
#define NIOS2_DCACHE_LINE_SIZE_LOG2 0
#define NIOS2_DCACHE_SIZE 0
#define NIOS2_EXCEPTION_ADDR 0x02000020
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INST_ADDR_WIDTH 0x1b
#define NIOS2_RESET_ADDR 0x04001000


/*
 * CTRL_I2C configuration
 *
 */

#define ALT_MODULE_CLASS_CTRL_I2C altera_avalon_pio
#define CTRL_I2C_BASE 0x40021b0
#define CTRL_I2C_BIT_CLEARING_EDGE_REGISTER 0
#define CTRL_I2C_BIT_MODIFYING_OUTPUT_REGISTER 0
#define CTRL_I2C_CAPTURE 0
#define CTRL_I2C_DATA_WIDTH 7
#define CTRL_I2C_DO_TEST_BENCH_WIRING 0
#define CTRL_I2C_DRIVEN_SIM_VALUE 0
#define CTRL_I2C_EDGE_TYPE "NONE"
#define CTRL_I2C_FREQ 100000000
#define CTRL_I2C_HAS_IN 0
#define CTRL_I2C_HAS_OUT 1
#define CTRL_I2C_HAS_TRI 0
#define CTRL_I2C_IRQ -1
#define CTRL_I2C_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CTRL_I2C_IRQ_TYPE "NONE"
#define CTRL_I2C_NAME "/dev/CTRL_I2C"
#define CTRL_I2C_RESET_VALUE 0
#define CTRL_I2C_SPAN 16
#define CTRL_I2C_TYPE "altera_avalon_pio"


/*
 * DATA_IN_I2C configuration
 *
 */

#define ALT_MODULE_CLASS_DATA_IN_I2C altera_avalon_pio
#define DATA_IN_I2C_BASE 0x4002190
#define DATA_IN_I2C_BIT_CLEARING_EDGE_REGISTER 0
#define DATA_IN_I2C_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DATA_IN_I2C_CAPTURE 0
#define DATA_IN_I2C_DATA_WIDTH 8
#define DATA_IN_I2C_DO_TEST_BENCH_WIRING 0
#define DATA_IN_I2C_DRIVEN_SIM_VALUE 0
#define DATA_IN_I2C_EDGE_TYPE "NONE"
#define DATA_IN_I2C_FREQ 100000000
#define DATA_IN_I2C_HAS_IN 1
#define DATA_IN_I2C_HAS_OUT 0
#define DATA_IN_I2C_HAS_TRI 0
#define DATA_IN_I2C_IRQ -1
#define DATA_IN_I2C_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DATA_IN_I2C_IRQ_TYPE "NONE"
#define DATA_IN_I2C_NAME "/dev/DATA_IN_I2C"
#define DATA_IN_I2C_RESET_VALUE 0
#define DATA_IN_I2C_SPAN 16
#define DATA_IN_I2C_TYPE "altera_avalon_pio"


/*
 * DATA_OUT_I2C configuration
 *
 */

#define ALT_MODULE_CLASS_DATA_OUT_I2C altera_avalon_pio
#define DATA_OUT_I2C_BASE 0x40021a0
#define DATA_OUT_I2C_BIT_CLEARING_EDGE_REGISTER 0
#define DATA_OUT_I2C_BIT_MODIFYING_OUTPUT_REGISTER 0
#define DATA_OUT_I2C_CAPTURE 0
#define DATA_OUT_I2C_DATA_WIDTH 8
#define DATA_OUT_I2C_DO_TEST_BENCH_WIRING 0
#define DATA_OUT_I2C_DRIVEN_SIM_VALUE 0
#define DATA_OUT_I2C_EDGE_TYPE "NONE"
#define DATA_OUT_I2C_FREQ 100000000
#define DATA_OUT_I2C_HAS_IN 0
#define DATA_OUT_I2C_HAS_OUT 1
#define DATA_OUT_I2C_HAS_TRI 0
#define DATA_OUT_I2C_IRQ -1
#define DATA_OUT_I2C_IRQ_INTERRUPT_CONTROLLER_ID -1
#define DATA_OUT_I2C_IRQ_TYPE "NONE"
#define DATA_OUT_I2C_NAME "/dev/DATA_OUT_I2C"
#define DATA_OUT_I2C_RESET_VALUE 0
#define DATA_OUT_I2C_SPAN 16
#define DATA_OUT_I2C_TYPE "altera_avalon_pio"


/*
 * DIST1 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST1 altera_avalon_uart
#define DIST1_BASE 0x40020e0
#define DIST1_BAUD 9600
#define DIST1_DATA_BITS 8
#define DIST1_FIXED_BAUD 1
#define DIST1_FREQ 100000000
#define DIST1_IRQ 8
#define DIST1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST1_NAME "/dev/DIST1"
#define DIST1_PARITY 'N'
#define DIST1_SIM_CHAR_STREAM ""
#define DIST1_SIM_TRUE_BAUD 0
#define DIST1_SPAN 32
#define DIST1_STOP_BITS 1
#define DIST1_SYNC_REG_DEPTH 2
#define DIST1_TYPE "altera_avalon_uart"
#define DIST1_USE_CTS_RTS 0
#define DIST1_USE_EOP_REGISTER 0


/*
 * DIST2 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST2 altera_avalon_uart
#define DIST2_BASE 0x40020c0
#define DIST2_BAUD 9600
#define DIST2_DATA_BITS 8
#define DIST2_FIXED_BAUD 1
#define DIST2_FREQ 100000000
#define DIST2_IRQ 9
#define DIST2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST2_NAME "/dev/DIST2"
#define DIST2_PARITY 'N'
#define DIST2_SIM_CHAR_STREAM ""
#define DIST2_SIM_TRUE_BAUD 0
#define DIST2_SPAN 32
#define DIST2_STOP_BITS 1
#define DIST2_SYNC_REG_DEPTH 2
#define DIST2_TYPE "altera_avalon_uart"
#define DIST2_USE_CTS_RTS 0
#define DIST2_USE_EOP_REGISTER 0


/*
 * DIST3 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST3 altera_avalon_uart
#define DIST3_BASE 0x40020a0
#define DIST3_BAUD 9600
#define DIST3_DATA_BITS 8
#define DIST3_FIXED_BAUD 1
#define DIST3_FREQ 100000000
#define DIST3_IRQ 10
#define DIST3_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST3_NAME "/dev/DIST3"
#define DIST3_PARITY 'N'
#define DIST3_SIM_CHAR_STREAM ""
#define DIST3_SIM_TRUE_BAUD 0
#define DIST3_SPAN 32
#define DIST3_STOP_BITS 1
#define DIST3_SYNC_REG_DEPTH 2
#define DIST3_TYPE "altera_avalon_uart"
#define DIST3_USE_CTS_RTS 0
#define DIST3_USE_EOP_REGISTER 0


/*
 * DIST4 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST4 altera_avalon_uart
#define DIST4_BASE 0x4002080
#define DIST4_BAUD 9600
#define DIST4_DATA_BITS 8
#define DIST4_FIXED_BAUD 1
#define DIST4_FREQ 100000000
#define DIST4_IRQ 11
#define DIST4_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST4_NAME "/dev/DIST4"
#define DIST4_PARITY 'N'
#define DIST4_SIM_CHAR_STREAM ""
#define DIST4_SIM_TRUE_BAUD 0
#define DIST4_SPAN 32
#define DIST4_STOP_BITS 1
#define DIST4_SYNC_REG_DEPTH 2
#define DIST4_TYPE "altera_avalon_uart"
#define DIST4_USE_CTS_RTS 0
#define DIST4_USE_EOP_REGISTER 0


/*
 * DIST5 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST5 altera_avalon_uart
#define DIST5_BASE 0x4002060
#define DIST5_BAUD 9600
#define DIST5_DATA_BITS 8
#define DIST5_FIXED_BAUD 1
#define DIST5_FREQ 100000000
#define DIST5_IRQ 12
#define DIST5_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST5_NAME "/dev/DIST5"
#define DIST5_PARITY 'N'
#define DIST5_SIM_CHAR_STREAM ""
#define DIST5_SIM_TRUE_BAUD 0
#define DIST5_SPAN 32
#define DIST5_STOP_BITS 1
#define DIST5_SYNC_REG_DEPTH 2
#define DIST5_TYPE "altera_avalon_uart"
#define DIST5_USE_CTS_RTS 0
#define DIST5_USE_EOP_REGISTER 0


/*
 * DIST6 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST6 altera_avalon_uart
#define DIST6_BASE 0x4002040
#define DIST6_BAUD 9600
#define DIST6_DATA_BITS 8
#define DIST6_FIXED_BAUD 1
#define DIST6_FREQ 100000000
#define DIST6_IRQ 13
#define DIST6_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST6_NAME "/dev/DIST6"
#define DIST6_PARITY 'N'
#define DIST6_SIM_CHAR_STREAM ""
#define DIST6_SIM_TRUE_BAUD 0
#define DIST6_SPAN 32
#define DIST6_STOP_BITS 1
#define DIST6_SYNC_REG_DEPTH 2
#define DIST6_TYPE "altera_avalon_uart"
#define DIST6_USE_CTS_RTS 0
#define DIST6_USE_EOP_REGISTER 0


/*
 * DIST7 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST7 altera_avalon_uart
#define DIST7_BASE 0x4002020
#define DIST7_BAUD 9600
#define DIST7_DATA_BITS 8
#define DIST7_FIXED_BAUD 1
#define DIST7_FREQ 100000000
#define DIST7_IRQ 14
#define DIST7_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST7_NAME "/dev/DIST7"
#define DIST7_PARITY 'N'
#define DIST7_SIM_CHAR_STREAM ""
#define DIST7_SIM_TRUE_BAUD 0
#define DIST7_SPAN 32
#define DIST7_STOP_BITS 1
#define DIST7_SYNC_REG_DEPTH 2
#define DIST7_TYPE "altera_avalon_uart"
#define DIST7_USE_CTS_RTS 0
#define DIST7_USE_EOP_REGISTER 0


/*
 * DIST8 configuration
 *
 */

#define ALT_MODULE_CLASS_DIST8 altera_avalon_uart
#define DIST8_BASE 0x4002000
#define DIST8_BAUD 9600
#define DIST8_DATA_BITS 8
#define DIST8_FIXED_BAUD 1
#define DIST8_FREQ 100000000
#define DIST8_IRQ 15
#define DIST8_IRQ_INTERRUPT_CONTROLLER_ID 0
#define DIST8_NAME "/dev/DIST8"
#define DIST8_PARITY 'N'
#define DIST8_SIM_CHAR_STREAM ""
#define DIST8_SIM_TRUE_BAUD 0
#define DIST8_SPAN 32
#define DIST8_STOP_BITS 1
#define DIST8_SYNC_REG_DEPTH 2
#define DIST8_TYPE "altera_avalon_uart"
#define DIST8_USE_CTS_RTS 0
#define DIST8_USE_EOP_REGISTER 0


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_EPCS_FLASH_CONTROLLER
#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_SYSID_QSYS
#define __ALTERA_AVALON_TIMER
#define __ALTERA_AVALON_UART
#define __ALTERA_NIOS2_QSYS
#define __ALTERA_UP_AVALON_ACCELEROMETER_SPI
#define __ALTERA_UP_AVALON_DE0_NANO_ADC
#define __ALTPLL


/*
 * ENCODER_INT configuration
 *
 */

#define ALT_MODULE_CLASS_ENCODER_INT altera_avalon_pio
#define ENCODER_INT_BASE 0x4002200
#define ENCODER_INT_BIT_CLEARING_EDGE_REGISTER 0
#define ENCODER_INT_BIT_MODIFYING_OUTPUT_REGISTER 0
#define ENCODER_INT_CAPTURE 1
#define ENCODER_INT_DATA_WIDTH 4
#define ENCODER_INT_DO_TEST_BENCH_WIRING 0
#define ENCODER_INT_DRIVEN_SIM_VALUE 0
#define ENCODER_INT_EDGE_TYPE "RISING"
#define ENCODER_INT_FREQ 100000000
#define ENCODER_INT_HAS_IN 1
#define ENCODER_INT_HAS_OUT 0
#define ENCODER_INT_HAS_TRI 0
#define ENCODER_INT_IRQ 4
#define ENCODER_INT_IRQ_INTERRUPT_CONTROLLER_ID 0
#define ENCODER_INT_IRQ_TYPE "EDGE"
#define ENCODER_INT_NAME "/dev/ENCODER_INT"
#define ENCODER_INT_RESET_VALUE 0
#define ENCODER_INT_SPAN 16
#define ENCODER_INT_TYPE "altera_avalon_pio"


/*
 * ENCODER_NORMAL configuration
 *
 */

#define ALT_MODULE_CLASS_ENCODER_NORMAL altera_avalon_pio
#define ENCODER_NORMAL_BASE 0x40021f0
#define ENCODER_NORMAL_BIT_CLEARING_EDGE_REGISTER 0
#define ENCODER_NORMAL_BIT_MODIFYING_OUTPUT_REGISTER 0
#define ENCODER_NORMAL_CAPTURE 0
#define ENCODER_NORMAL_DATA_WIDTH 4
#define ENCODER_NORMAL_DO_TEST_BENCH_WIRING 0
#define ENCODER_NORMAL_DRIVEN_SIM_VALUE 0
#define ENCODER_NORMAL_EDGE_TYPE "NONE"
#define ENCODER_NORMAL_FREQ 100000000
#define ENCODER_NORMAL_HAS_IN 1
#define ENCODER_NORMAL_HAS_OUT 0
#define ENCODER_NORMAL_HAS_TRI 0
#define ENCODER_NORMAL_IRQ -1
#define ENCODER_NORMAL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ENCODER_NORMAL_IRQ_TYPE "NONE"
#define ENCODER_NORMAL_NAME "/dev/ENCODER_NORMAL"
#define ENCODER_NORMAL_RESET_VALUE 0
#define ENCODER_NORMAL_SPAN 16
#define ENCODER_NORMAL_TYPE "altera_avalon_pio"


/*
 * EPCS configuration
 *
 */

#define ALT_MODULE_CLASS_EPCS altera_avalon_epcs_flash_controller
#define EPCS_BASE 0x4001000
#define EPCS_IRQ 0
#define EPCS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define EPCS_NAME "/dev/EPCS"
#define EPCS_REGISTER_OFFSET 1024
#define EPCS_SPAN 2048
#define EPCS_TYPE "altera_avalon_epcs_flash_controller"


/*
 * FLAG_I2C configuration
 *
 */

#define ALT_MODULE_CLASS_FLAG_I2C altera_avalon_pio
#define FLAG_I2C_BASE 0x4002180
#define FLAG_I2C_BIT_CLEARING_EDGE_REGISTER 0
#define FLAG_I2C_BIT_MODIFYING_OUTPUT_REGISTER 0
#define FLAG_I2C_CAPTURE 0
#define FLAG_I2C_DATA_WIDTH 2
#define FLAG_I2C_DO_TEST_BENCH_WIRING 0
#define FLAG_I2C_DRIVEN_SIM_VALUE 0
#define FLAG_I2C_EDGE_TYPE "NONE"
#define FLAG_I2C_FREQ 100000000
#define FLAG_I2C_HAS_IN 1
#define FLAG_I2C_HAS_OUT 0
#define FLAG_I2C_HAS_TRI 0
#define FLAG_I2C_IRQ -1
#define FLAG_I2C_IRQ_INTERRUPT_CONTROLLER_ID -1
#define FLAG_I2C_IRQ_TYPE "NONE"
#define FLAG_I2C_NAME "/dev/FLAG_I2C"
#define FLAG_I2C_RESET_VALUE 0
#define FLAG_I2C_SPAN 16
#define FLAG_I2C_TYPE "altera_avalon_pio"


/*
 * GPS configuration
 *
 */

#define ALT_MODULE_CLASS_GPS altera_avalon_uart
#define GPS_BASE 0x4002120
#define GPS_BAUD 9600
#define GPS_DATA_BITS 8
#define GPS_FIXED_BAUD 1
#define GPS_FREQ 100000000
#define GPS_IRQ 6
#define GPS_IRQ_INTERRUPT_CONTROLLER_ID 0
#define GPS_NAME "/dev/GPS"
#define GPS_PARITY 'N'
#define GPS_SIM_CHAR_STREAM ""
#define GPS_SIM_TRUE_BAUD 0
#define GPS_SPAN 32
#define GPS_STOP_BITS 1
#define GPS_SYNC_REG_DEPTH 2
#define GPS_TYPE "altera_avalon_uart"
#define GPS_USE_CTS_RTS 0
#define GPS_USE_EOP_REGISTER 0


/*
 * JTAG configuration
 *
 */

#define ALT_MODULE_CLASS_JTAG altera_avalon_jtag_uart
#define JTAG_BASE 0x4002250
#define JTAG_IRQ 2
#define JTAG_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_NAME "/dev/JTAG"
#define JTAG_READ_DEPTH 64
#define JTAG_READ_THRESHOLD 8
#define JTAG_SPAN 8
#define JTAG_TYPE "altera_avalon_jtag_uart"
#define JTAG_WRITE_DEPTH 64
#define JTAG_WRITE_THRESHOLD 8


/*
 * KEY configuration
 *
 */

#define ALT_MODULE_CLASS_KEY altera_avalon_pio
#define KEY_BASE 0x4002220
#define KEY_BIT_CLEARING_EDGE_REGISTER 0
#define KEY_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEY_CAPTURE 1
#define KEY_DATA_WIDTH 2
#define KEY_DO_TEST_BENCH_WIRING 0
#define KEY_DRIVEN_SIM_VALUE 0
#define KEY_EDGE_TYPE "FALLING"
#define KEY_FREQ 100000000
#define KEY_HAS_IN 1
#define KEY_HAS_OUT 0
#define KEY_HAS_TRI 0
#define KEY_IRQ 3
#define KEY_IRQ_INTERRUPT_CONTROLLER_ID 0
#define KEY_IRQ_TYPE "EDGE"
#define KEY_NAME "/dev/KEY"
#define KEY_RESET_VALUE 0
#define KEY_SPAN 16
#define KEY_TYPE "altera_avalon_pio"


/*
 * LED configuration
 *
 */

#define ALT_MODULE_CLASS_LED altera_avalon_pio
#define LED_BASE 0x4002240
#define LED_BIT_CLEARING_EDGE_REGISTER 0
#define LED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define LED_CAPTURE 0
#define LED_DATA_WIDTH 8
#define LED_DO_TEST_BENCH_WIRING 0
#define LED_DRIVEN_SIM_VALUE 0
#define LED_EDGE_TYPE "NONE"
#define LED_FREQ 100000000
#define LED_HAS_IN 0
#define LED_HAS_OUT 1
#define LED_HAS_TRI 0
#define LED_IRQ -1
#define LED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define LED_IRQ_TYPE "NONE"
#define LED_NAME "/dev/LED"
#define LED_RESET_VALUE 0
#define LED_SPAN 16
#define LED_TYPE "altera_avalon_pio"


/*
 * MOTORES configuration
 *
 */

#define ALT_MODULE_CLASS_MOTORES altera_avalon_pio
#define MOTORES_BASE 0x40021e0
#define MOTORES_BIT_CLEARING_EDGE_REGISTER 0
#define MOTORES_BIT_MODIFYING_OUTPUT_REGISTER 0
#define MOTORES_CAPTURE 0
#define MOTORES_DATA_WIDTH 6
#define MOTORES_DO_TEST_BENCH_WIRING 0
#define MOTORES_DRIVEN_SIM_VALUE 0
#define MOTORES_EDGE_TYPE "NONE"
#define MOTORES_FREQ 100000000
#define MOTORES_HAS_IN 0
#define MOTORES_HAS_OUT 1
#define MOTORES_HAS_TRI 0
#define MOTORES_IRQ -1
#define MOTORES_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MOTORES_IRQ_TYPE "NONE"
#define MOTORES_NAME "/dev/MOTORES"
#define MOTORES_RESET_VALUE 0
#define MOTORES_SPAN 16
#define MOTORES_TYPE "altera_avalon_pio"


/*
 * PWM1 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM1 altera_avalon_pio
#define PWM1_BASE 0x40021d0
#define PWM1_BIT_CLEARING_EDGE_REGISTER 0
#define PWM1_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PWM1_CAPTURE 0
#define PWM1_DATA_WIDTH 8
#define PWM1_DO_TEST_BENCH_WIRING 0
#define PWM1_DRIVEN_SIM_VALUE 0
#define PWM1_EDGE_TYPE "NONE"
#define PWM1_FREQ 100000000
#define PWM1_HAS_IN 0
#define PWM1_HAS_OUT 1
#define PWM1_HAS_TRI 0
#define PWM1_IRQ -1
#define PWM1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM1_IRQ_TYPE "NONE"
#define PWM1_NAME "/dev/PWM1"
#define PWM1_RESET_VALUE 0
#define PWM1_SPAN 16
#define PWM1_TYPE "altera_avalon_pio"


/*
 * PWM2 configuration
 *
 */

#define ALT_MODULE_CLASS_PWM2 altera_avalon_pio
#define PWM2_BASE 0x40021c0
#define PWM2_BIT_CLEARING_EDGE_REGISTER 0
#define PWM2_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PWM2_CAPTURE 0
#define PWM2_DATA_WIDTH 8
#define PWM2_DO_TEST_BENCH_WIRING 0
#define PWM2_DRIVEN_SIM_VALUE 0
#define PWM2_EDGE_TYPE "NONE"
#define PWM2_FREQ 100000000
#define PWM2_HAS_IN 0
#define PWM2_HAS_OUT 1
#define PWM2_HAS_TRI 0
#define PWM2_IRQ -1
#define PWM2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PWM2_IRQ_TYPE "NONE"
#define PWM2_NAME "/dev/PWM2"
#define PWM2_RESET_VALUE 0
#define PWM2_SPAN 16
#define PWM2_TYPE "altera_avalon_pio"


/*
 * SDRAM configuration
 *
 */

#define ALT_MODULE_CLASS_SDRAM altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x2000000
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/SDRAM"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x18
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 9
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 13
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 33554432
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * SW configuration
 *
 */

#define ALT_MODULE_CLASS_SW altera_avalon_pio
#define SW_BASE 0x4002230
#define SW_BIT_CLEARING_EDGE_REGISTER 0
#define SW_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SW_CAPTURE 0
#define SW_DATA_WIDTH 4
#define SW_DO_TEST_BENCH_WIRING 0
#define SW_DRIVEN_SIM_VALUE 0
#define SW_EDGE_TYPE "NONE"
#define SW_FREQ 100000000
#define SW_HAS_IN 1
#define SW_HAS_OUT 0
#define SW_HAS_TRI 0
#define SW_IRQ -1
#define SW_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SW_IRQ_TYPE "NONE"
#define SW_NAME "/dev/SW"
#define SW_RESET_VALUE 0
#define SW_SPAN 16
#define SW_TYPE "altera_avalon_pio"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone IV E"
#define ALT_IRQ_BASE NULL
#define ALT_LEGACY_INTERRUPT_API_PRESENT
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/JTAG"
#define ALT_STDERR_BASE 0x4002250
#define ALT_STDERR_DEV JTAG
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/JTAG"
#define ALT_STDIN_BASE 0x4002250
#define ALT_STDIN_DEV JTAG
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/JTAG"
#define ALT_STDOUT_BASE 0x4002250
#define ALT_STDOUT_DEV JTAG
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "NIOS"


/*
 * XBEE configuration
 *
 */

#define ALT_MODULE_CLASS_XBEE altera_avalon_uart
#define XBEE_BASE 0x4002100
#define XBEE_BAUD 9600
#define XBEE_DATA_BITS 8
#define XBEE_FIXED_BAUD 1
#define XBEE_FREQ 100000000
#define XBEE_IRQ 7
#define XBEE_IRQ_INTERRUPT_CONTROLLER_ID 0
#define XBEE_NAME "/dev/XBEE"
#define XBEE_PARITY 'N'
#define XBEE_SIM_CHAR_STREAM ""
#define XBEE_SIM_TRUE_BAUD 0
#define XBEE_SPAN 32
#define XBEE_STOP_BITS 1
#define XBEE_SYNC_REG_DEPTH 2
#define XBEE_TYPE "altera_avalon_uart"
#define XBEE_USE_CTS_RTS 0
#define XBEE_USE_EOP_REGISTER 0


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK SYS_TIMER
#define ALT_TIMESTAMP_CLK none


/*
 * sys_id configuration
 *
 */

#define ALT_MODULE_CLASS_sys_id altera_avalon_sysid_qsys
#define SYS_ID_BASE 0x4002258
#define SYS_ID_ID 0
#define SYS_ID_IRQ -1
#define SYS_ID_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYS_ID_NAME "/dev/sys_id"
#define SYS_ID_SPAN 8
#define SYS_ID_TIMESTAMP 1569347937
#define SYS_ID_TYPE "altera_avalon_sysid_qsys"


/*
 * sys_pll configuration
 *
 */

#define ALT_MODULE_CLASS_sys_pll altpll
#define SYS_PLL_BASE 0x4002210
#define SYS_PLL_IRQ -1
#define SYS_PLL_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYS_PLL_NAME "/dev/sys_pll"
#define SYS_PLL_SPAN 16
#define SYS_PLL_TYPE "altpll"


/*
 * sys_timer configuration
 *
 */

#define ALT_MODULE_CLASS_sys_timer altera_avalon_timer
#define SYS_TIMER_ALWAYS_RUN 0
#define SYS_TIMER_BASE 0x4002160
#define SYS_TIMER_COUNTER_SIZE 32
#define SYS_TIMER_FIXED_PERIOD 0
#define SYS_TIMER_FREQ 100000000
#define SYS_TIMER_IRQ 1
#define SYS_TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYS_TIMER_LOAD_VALUE 99999
#define SYS_TIMER_MULT 0.0010
#define SYS_TIMER_NAME "/dev/sys_timer"
#define SYS_TIMER_PERIOD 1
#define SYS_TIMER_PERIOD_UNITS "ms"
#define SYS_TIMER_RESET_OUTPUT 0
#define SYS_TIMER_SNAPSHOT 1
#define SYS_TIMER_SPAN 32
#define SYS_TIMER_TICKS_PER_SEC 1000.0
#define SYS_TIMER_TIMEOUT_PULSE_OUTPUT 0
#define SYS_TIMER_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
