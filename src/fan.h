#pragma once

void InitFan();
void FanSpeed(unsigned int fanSpeed);
#define FAN_ON GpioDataRegs.GPBSET.bit.GPIO34 = 1
#define FAN_OFF GpioDataRegs.GPBCLEAR.bit.GPIO34 = 1
