/**
 * @title   MotionBoard project v2.0
 * @file    accelerometer.c
 * @brief   This module allow to control tricolour leds
 * @author  Yves Chevallier <yves.chevallier@kalios.ch>
 * @svn     $Id: accelerometer.c 490 2008-11-15 14:24:53Z yves $
 */

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "global.h"

////////////////////////////////////////////////////////////////////////////////
/// Initialize leds                                                             
////////////////////////////////////////////////////////////////////////////////
void 
Init_accelerometer(void)
{ 
    EALLOW; 

    // No peripheral assigned
    GpioCtrlRegs.GPAMUX2.bit.GPIO22 = 0; 

    // Set direction to output
    GpioCtrlRegs.GPADIR.bit.GPIO22 = 1; 

    // Clear bit
    GpioDataRegs.GPACLEAR.bit.GPIO22 = 1;    

    EDIS;
}
