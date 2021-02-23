#include "leds.h"

#include "common.h"
#include "device.h"

void InitLeds(void)
{
    EALLOW;

    // GPIO27..29 = no peripheral assigned
    GpioCtrlRegs.GPAMUX2.bit.GPIO27 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO29 = 0;

    // GPIO27..29 = output
    GpioCtrlRegs.GPADIR.bit.GPIO27 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 1;
    GpioCtrlRegs.GPADIR.bit.GPIO29 = 1;

    // GPIO27..29 = 0
    GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;
    GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;

    EDIS;
}

/**
 * Control all 3 leds.
 * @param value State of leds
 */
void Leds_set(Uint8 value)
{
    // Red Led
    if (value & 0x01)
        GpioDataRegs.GPASET.bit.GPIO27 = 1;
    else
        GpioDataRegs.GPACLEAR.bit.GPIO27 = 1;

    // Yellow Led
    if (value & 0x02)
        GpioDataRegs.GPASET.bit.GPIO28 = 1;
    else
        GpioDataRegs.GPACLEAR.bit.GPIO28 = 1;

    // Green Led
    if (value & 0x04)
        GpioDataRegs.GPASET.bit.GPIO29 = 1;
    else
        GpioDataRegs.GPACLEAR.bit.GPIO29 = 1;
}

/**
 * Control red led.
 * @param value State of the red led
 */
void Leds_red(boolean value) { GpioDataRegs.GPADAT.bit.GPIO27 = value; }

/**
 * Control yellow led.
 * @param value State of the yellow led
 */
void Leds_yellow(boolean value) { GpioDataRegs.GPADAT.bit.GPIO28 = value; }

/**
 * Control green led.
 * @param value State of the green led
 */
void Leds_green(boolean value) { GpioDataRegs.GPADAT.bit.GPIO29 = value; }

/**
 * Toggle red led.
 */
void Leds_red_toggle() { GpioDataRegs.GPATOGGLE.bit.GPIO27 = 1; }

/**
 * Toggle yellow led.
 */
void Leds_yellow_toggle() { GpioDataRegs.GPATOGGLE.bit.GPIO28 = 1; }

/**
 * Toggle yellow led.
 */
void Leds_green_toggle() { GpioDataRegs.GPATOGGLE.bit.GPIO29 = 1; }
