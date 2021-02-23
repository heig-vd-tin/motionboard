#include "device.h"
#include "common.h"
#include "IQmathLib.h"

// Global Configuration
extern volatile globalConfiguration global;
extern eqep motorA;
extern eqep motorB;

factor=0;

#define _IQ15TO22(x) ((x)<<7)

void
InitXY()
{
  int32 num, den;

  num = global.mca.encoder_resolution*4*global.mca.gear_num;
  den = _IQmpy(_IQ(3.141592), global.mca.gear_den);
  den = _IQmpy(den, global.mca.wheel_diameter);
  // A finir...
}

int32
ticks2distance(int32 position_mm)
{
  uint16 dA = 0, dB = 0;
  _iq22 dAlpha, dDelta;
  _iq22 alpha;
  _iq22 dX, dY;
  _iq entrax;

  dA = motorA.position - motorA.oldposition;
  dB = motorB.position - motorB.oldposition;

  dAlpha = (((_iq)dA-_iq(dB))<<22)/2;
  dDelta = (((_iq)dA+_iq(dR))<<22)/2;

  entrax = _IQ15TO22(global.xyp.entrax);
  alpha  = _IQdiv(dAlpha, entrax);

  dX = _IQ22mpy(_IQ22cos(alpha), dDelta);
  dY = _IQ22mpy(_IQ22sin(alpha), dDelta);

  global.xyp.x += dX;
  global.xyp.y += dY;
}
