#!/bin/sh
#
# This file was automatically generated.
#
# It can be overwritten by nios2-flash-programmer-generate or nios2-flash-programmer-gui.
#

#
# Converting SOF File: C:\Quartus\Embedded\ROVER\ROVER.sof to: "..\flash/ROVER_EPCS.flash"
#
sof2flash --input="C:/Quartus/Embedded/ROVER/ROVER.sof" --output="../flash/ROVER_EPCS.flash" --epcs 

#
# Programming File: "..\flash/ROVER_EPCS.flash" To Device: EPCS
#
nios2-flash-programmer "../flash/ROVER_EPCS.flash" --base=0x4001000 --epcs --sidp=0x4002258 --id=0x0 --timestamp=1569347937 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program 

#
# Converting ELF File: C:\Quartus\Embedded\ROVER\Software\Rover5v2\Rover5v2.elf to: "..\flash/Rover5v2_EPCS.flash"
#
elf2flash --input="Rover5v2.elf" --output="../flash/Rover5v2_EPCS.flash" --epcs --after="../flash/ROVER_EPCS.flash" 

#
# Programming File: "..\flash/Rover5v2_EPCS.flash" To Device: EPCS
#
nios2-flash-programmer "../flash/Rover5v2_EPCS.flash" --base=0x4001000 --epcs --sidp=0x4002258 --id=0x0 --timestamp=1569347937 --device=1 --instance=0 '--cable=USB-Blaster on localhost [USB-0]' --program 

