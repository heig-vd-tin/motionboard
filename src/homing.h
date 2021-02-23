#pragma once

#include "global.h"

#define FW_A GpioDataRegs.GPBDAT.bit.GPIO54    // IO3
#define BW_A GpioDataRegs.GPBDAT.bit.GPIO53    // IO4
#define HOME_A GpioDataRegs.GPBDAT.bit.GPIO52  // IO5
#define FW_B GpioDataRegs.GPBDAT.bit.GPIO51    // IO6
#define BW_B GpioDataRegs.GPBDAT.bit.GPIO50    // IO7
#define HOME_B GpioDataRegs.GPBDAT.bit.GPIO49  // IO8

static inline void InitLimits(void)
{
    EALLOW;
    GpioCtrlRegs.GPBPUD.bit.GPIO49 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO52 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;
    GpioCtrlRegs.GPBPUD.bit.GPIO54 = DIS_PULLUP;
    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 0;
    EDIS;
}
