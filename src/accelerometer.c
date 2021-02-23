/**
 * Accelerometer
 */
#include "global.h"

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
