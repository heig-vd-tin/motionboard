#pragma once

static inline void Init_leds(void);
static inline void Leds_set(Uint16 value);

static inline void Leds_red(bool value);
static inline void Leds_yellow(bool value);
static inline void Leds_green(bool value);

static inline void Leds_yellow_toggle(void);
static inline void Leds_red_toggle(void);
static inline void Leds_green_toggle(void);

static inline void
Init_leds(void)
{
    EALLOW;

    // No peripheral assigned
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;

    // Set direction to output
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;

    // Clear leds
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;

    EDIS;
}

static inline void
Leds_set(Uint16 value)
{
  // Red Led
  if(value & 0x01)
    GpioDataRegs.GPBSET.bit.GPIO57 = 1;
  else
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;

  // Yellow Led
  if(value & 0x02)
    GpioDataRegs.GPBSET.bit.GPIO55 = 1;
  else
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;

  // Green Led
  if(value & 0x04)
    GpioDataRegs.GPBSET.bit.GPIO56 = 1;
  else
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;
}

static inline void
Leds_red(bool value)
{
  GpioDataRegs.GPBDAT.bit.GPIO57 = value;
}

static inline void
Leds_yellow(bool value)
{
  GpioDataRegs.GPBDAT.bit.GPIO55 = value;
}

static inline void
Leds_green(bool value)
{
  GpioDataRegs.GPBDAT.bit.GPIO56 = value;
}

static inline void
Leds_red_toggle()
{
  GpioDataRegs.GPBTOGGLE.bit.GPIO57 = 1;
}

static inline void
Leds_yellow_toggle()
{
  GpioDataRegs.GPBTOGGLE.bit.GPIO55 = 1;
}

static inline void
Leds_green_toggle()
{
  GpioDataRegs.GPBTOGGLE.bit.GPIO56 = 1;
}
