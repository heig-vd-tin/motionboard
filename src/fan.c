#include "device.h"
#include "common.h"
#include "fan.h"

void
InitFan ()
{
  EALLOW;
  GpioCtrlRegs.GPAPUD.bit.GPIO14  = DIS_PULLUP;
  GpioCtrlRegs.GPBMUX1.bit.GPIO34 = MUX_GPIO34_GPIO34;
  GpioCtrlRegs.GPBDIR.bit.GPIO34  = GPIO_OUTPUT;
  EDIS;

  GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1;
}

void
FanSpeed(unsigned int fanSpeed)
{
  static unsigned int i;
  if(++i > fanSpeed)
  {
    FAN_OFF;
    if(i > 100)
      i=0;
  }
  else
    FAN_ON;
}
