#pragma once

#include "IQmathLib.h"

typedef struct
{
  // Maximum Values
  _iq18 accMax;
  _iq18 decMax;
  _iq18 velMax;

  // Nominal Values
  _iq18 acc;
  _iq18 dec;
  _iq18 speed;

  // Position information
  int64 pos_left;
  int64 pos_start;
  int64 pos_init;

  // Speed information (for internal use)
  volatile _iq18 old_speed;
  volatile _iq18 cur_speed;

  // Status Flags (for internal use)
  bool start_flag;
  bool busy_flag;
  bool direction;
  bool stop_flag;

} handle_bangbang;

#define BANGBANG_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0}

void Init_bangbang(handle_bangbang *h, int fech, int coder, int accMax, int decMax, long vMax);
void bangbang_mvt (handle_bangbang *h, int acceleration, int deceleration, int nom_speed, int64 position);
long bangbang_process (handle_bangbang *h, long position);
void bangbang_reset(handle_bangbang *h);
void bangbang_smooth_stop(handle_bangbang *h);
