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
/// @file     fan.c                                                             
/// @language ASCII/C                                                           
/// @svn      $Id: fan.c 136 2007-03-10 18:19:49Z ychevall@heig-vd.ch $         
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"    
#include "common.h"  
#include "fan.h"

////////////////////////////////////////////////////////////////////////////////
/// Initialize fan GPIO                                                         
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// Fan Speed                                                                   
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// Initialize fan GPIO                                                         
////////////////////////////////////////////////////////////////////////////////

