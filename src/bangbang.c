////////////////////////////////////////////////////////////////////////////////
///  _   _        _                            _                                
/// | | | |      |_|                          | |                               
/// | |_| |_____  _ _____           _     _ __| |                               
/// | |_  | ___ || |  _  \  _____  \ \  / // _  |                               
/// | | | | ____|| | |_| | (_____)  \ \/ /( (_| |                               
/// |_| |_|_____)|_|___  |           \__/  \____|                               
///                  __| | Haute Ecole d'Ingenieurs                             
///                 |___/  et de Gestion - Vaud                                 
///                                                                             
/// @title    Logiciel de contrôle de moteur pour la carte "motionboard"        
/// @context  Coupe suisse de robotique 2007                                    
/// @author   Y. Chevallier <nowox@kalios.ch>                                   
/// @file     bangbang.c                                                        
/// @language ASCII/C                                                           
/// @svn      $Id: main.c 154 2007-03-21 07:58:46Z ychevall@heig-vd.ch $        
///                                                                             
////////////////////////////////////////////////////////////////////////////////
       
////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   
#include "IQmathLib.h"
#include "mpyS.h"
#include "bangbang.h" 

volatile int i = 0; 
volatile int j = 0; 
volatile int k = 20; 

////////////////////////////////////////////////////////////////////////////////
/// Macros                                                                      
////////////////////////////////////////////////////////////////////////////////
#define _IQ6TO18(x)  ((x)<<12)
#define _IQ0TO17(x)  ((x)<<17)
#define _IQ18TO16(x) ((x)>>2)
#define _IQ18TO17(x) ((x)>>1)
#define _IQ16TO18(x) ((x)<<2)
#define _IQ17TO16(x) ((x)>>1)
#define _IQ18TO10(x) ((x)>>8)
#define _IQ10TO18(x) ((x)<<8)

////////////////////////////////////////////////////////////////////////////////
/// Initialize Parameters.                                                      
/// @param handle Handle of this bangbang calculation                           
/// @param accMax Maximum acceleration  [tr/sec^2]  0 < x < 1500                
/// @param decMax Maximum deceleration  [tr/sec^2]  0 < x < 1500                
/// @param vMax   Maximum velocity      [tr/min]    0 < x < 20000               
/// @param coder  Coder Resolution      [ticks]     128 < x < 2048              
/// @param fech   Frequency             [Hz]        2000 < x < 8000             
////////////////////////////////////////////////////////////////////////////////
_iq tmp0,tmp1, tmp2, tmp3,tmp4;  
void
Init_bangbang(handle_bangbang *h, int fech, int coder, int accMax, int decMax, long vMax)
{ 
  long acc, dec, vel, code, fe; 

  // Limitation 
  acc  = (long)(accMax >= 3500)?3500:accMax; 
  dec  = (long)(decMax >= 3500)?3500:decMax; 
  vel  = (long)(vMax>100000)?100000:vMax; 
  code = (long)(coder >= 5000)?5000:coder;
  fe   = (long)(fech <= 2000)?2000:fech; 
  fe   = (long)(fech >= 8000)?8000:fech;
   
  // Maximum Acceleration (IQ27)
  tmp0 = _IQ6mpy(acc<<6, code<<6);        // IQ6
  tmp1 = _IQ6TO18(_IQ6div(tmp0, fe<<6));  // IQ18
  tmp2 = _IQ18div(tmp1, fe<<18);          // IQ18
  tmp3 = _IQ18mpy(tmp2, _IQ18(4));        // IQ18
  h->accMax = tmp3; 

  // Maximum Deceleration (IQ27)
  tmp0 = _IQ6mpy(dec<<6, code<<6);        // IQ6
  tmp1 = _IQ6TO18(_IQ6div(tmp0, fe<<6));  // IQ18
  tmp2 = _IQ18div(tmp1, fe<<18);          // IQ18
  tmp3 = _IQ18mpy(tmp2, _IQ18(4));        // IQ18
  h->decMax = tmp3; 

  // Vmax = V/60*coder/fech
  tmp0 = _IQ7div(vel<<7, _IQ7(60));          // IQ7
  tmp1 = _IQ6mpy(tmp0>>1, ((_iq)code*4)<<6); // IQ6
  tmp2 = _IQ6div(tmp1, (_iq)fe<<6);          // IQ6
  h->velMax = tmp2<<12;                      // IQ18

  // Init parameters
  h->start_flag = 0;  
  h->busy_flag = 0; 
  h->direction = 0; 
  h->stop_flag = 0; 
  i = 0; 
}      

////////////////////////////////////////////////////////////////////////////////
/// Create BangBang                                                             
/// @param handle Handle of this bangbang calculation                           
/// @param acceleration Maximum acceleration [1.15]    (tr/sec^2)               
/// @param deceleration Maximum deceleration [1.15]    (tr/sec^2)               
/// @param nom_speed Nominal velocity [1.15]           (tr/min)                 
/// @param final_pos Final position [64.0]             (ticks)                  
/// @param current_pos Current position [64.0]         (ticks)                  
////////////////////////////////////////////////////////////////////////////////
void 
bangbang_mvt (handle_bangbang *h, int acceleration, int deceleration, int nom_speed, int64 position)
{
  h->acc   = _IQmpy(acceleration, h->accMax); 
  h->dec   = _IQmpy(deceleration, h->decMax);
  h->speed = _IQmpy(nom_speed,    h->velMax);
  
  // Sign Correction and Shift correction [64.0] -> [46.18]
  if(position < 0) 
  {
	h->pos_left  = (position-(2*position))<<18; 
	h->pos_start = (position-(2*position))<<18; 
    h->direction = 1; 
  }
  else
  {
  	h->pos_left  = position<<18;
	h->pos_start = position<<18;
    h->direction = 0; 
  }
       
  if(h->busy_flag != true)
  { 
  	h->old_speed = 0;
  	h->cur_speed = 0;
  } 
  h->busy_flag = true; 
  h->start_flag = true; 
  h->stop_flag = 0; 
}

////////////////////////////////////////////////////////////////////////////////
/// BangBang next position                                                      
/// @param handle Handle of this bangbang calculation                           
/// @param meas_position Position Measurement                                   
/// @return Next position                                                       
////////////////////////////////////////////////////////////////////////////////
_iq l, r;
_iq ppp = 0; 
bool latch = 0; 

long
bangbang_process (handle_bangbang *h, long position)
{
  if (h->busy_flag == 0) 	 
     return position; 
  if (h->start_flag == 1)
  {
     h->start_flag = 0; 
	 h->pos_init = position; 
  } 
  if (h->stop_flag == 1)
  {
	h->cur_speed -= h->dec;
	if(h->pos_left <= 0 || h->cur_speed < 0)
	{
		h->cur_speed = 0;
		h->busy_flag = 0;
		h->stop_flag = 0; 
		return position;
	} 
  }
  else if (h->pos_left <= 0 || h->cur_speed < 0)
  {
    h->cur_speed = 0;
    h->busy_flag = 0;
	h->pos_left = 0; 
	h->stop_flag = 0; 
  }	
  else
  {
    h->old_speed = h->cur_speed; 
    if(h->pos_left < _IQ18(15) && (long)h->pos_left < h->dec)
    {
      h->cur_speed = (long)h->pos_left;
      h->pos_left = 0;
    }
	else
	{
	  // Val max is 1 867 420
	  tmp0 = _IQ18TO10(h->cur_speed + h->cur_speed);
	  r = _IQ10mpy(tmp0, _IQ18TO10(h->cur_speed)); // IQ10

	  // Sature pos_left pour contenir dans l
	  if(h->pos_left>>18 > 309244) 
	     tmp0 = _IQ10(309244); // IQ10
	  else 
	     tmp0 = (long)(h->pos_left>>8);  // IQ10

      tmp1 = _IQ10mpy(tmp0, _IQ10(2));
      tmp2 = _IQ10mpy(tmp1, _IQ18TO10(h->dec));
      l = tmp2;

      if(l <= r)
      {
	     h->cur_speed = h->cur_speed - h->dec; 
      }		
	  else
	  {
		if(h->cur_speed + h->acc > h->speed)
		{
		  h->cur_speed = h->speed;
		}
		else
		{
		  h->cur_speed = h->cur_speed + h->acc; 
		}
	  }
	}
  }
  h->pos_left = h->pos_left - h->cur_speed; 

  // Sign Correction
  if(h->direction == 0)
	ppp =  ((h->pos_start - h->pos_left)>>18);
  else
	ppp = -((h->pos_start - h->pos_left)>>18);

  // Return position
  return (h->pos_init + ppp);   
}

////////////////////////////////////////////////////////////////////////////////
/// BangBang reset values                                                       
/// @param handle Handle of this bangbang calculation                           
/// @param meas_position Position Measurement                                   
/// @return Next position                                                       
////////////////////////////////////////////////////////////////////////////////
void 
bangbang_reset(handle_bangbang *h)
{
	h->cur_speed = 0;
	h->busy_flag = 0; 
	h->start_flag = 0; 
}

////////////////////////////////////////////////////////////////////////////////
/// BangBang smooth stop
/// @param handle Handle of this bangbang calculation                           
////////////////////////////////////////////////////////////////////////////////
void 
bangbang_smooth_stop(handle_bangbang *h)
{
	h->stop_flag = 1; 
} 

