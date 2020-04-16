#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting SOF File: C:\UPTCRover5v2\ROVER5\ROVER5.sof to: "..\flash/ROVER5_EPCS.flash"
#
sof2flash --input="C:/UPTCRover5v2/ROVER5/ROVER5.sof" --output="../flash/ROVER5_EPCS.flash" --epcs 

#
# Programming File: "..\flash/ROVER5_EPCS.flash" To Device: EPCS
#
nios2-flash-programmer "../flash/ROVER5_EPCS.flash" --base=0x4001000 --epcs --sidp=0x4002278 --id=0x0 --timestamp=1586536780 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program 

#
# Converting ELF File: C:\UPTCRover5v2\ROVER5\software\Rover5_FreeRTOS\Rover5_FreeRTOS.elf to: "..\flash/Rover5_FreeRTOS_EPCS.flash"
#
elf2flash --input="Rover5_FreeRTOS.elf" --output="../flash/Rover5_FreeRTOS_EPCS.flash" --epcs --after="../flash/ROVER5_EPCS.flash" 

#
# Programming File: "..\flash/Rover5_FreeRTOS_EPCS.flash" To Device: EPCS
#
nios2-flash-programmer "../flash/Rover5_FreeRTOS_EPCS.flash" --base=0x4001000 --epcs --sidp=0x4002278 --id=0x0 --timestamp=1586536780 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program 

