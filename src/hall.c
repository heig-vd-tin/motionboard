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
/// @file     hall.c                                                            
/// @language ASCII/C                                                           
/// @svn      $Id: hall.c 173 2007-03-31 22:49:25Z ychevall@heig-vd.ch $        
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   

////////////////////////////////////////////////////////////////////////////////
/// Global constants                                                            
////////////////////////////////////////////////////////////////////////////////
#define H1a (unsigned int)GpioDataRegs.GPADAT.bit.GPIO14
#define H2a (unsigned int)GpioDataRegs.GPADAT.bit.GPIO15
#define H3a (unsigned int)GpioDataRegs.GPADAT.bit.GPIO16

#define H1b (unsigned int)GpioDataRegs.GPADAT.bit.GPIO17
#define H2b (unsigned int)GpioDataRegs.GPADAT.bit.GPIO22
#define H3b (unsigned int)GpioDataRegs.GPADAT.bit.GPIO19

int OFFSET = 16385; 

static const int P6 = 0xEAA9; //0xD555; 
static const int P5 = 0xBFFF; //0xAAAA; 
static const int P4 = 0x9554; //0x8000; 
static const int P3 = 0x6AAA; //0x5555; 
static const int P2 = 0x3FFF; //0x2AAA; 
static const int P1 = 0x1555; //0x1555; 

////////////////////////////////////////////////////////////////////////////////
/// Initialize hall GPIO                                                        
////////////////////////////////////////////////////////////////////////////////
void 
InitHall ()
{
  EALLOW;  
  GpioCtrlRegs.GPAPUD.bit.GPIO14 = DIS_PULLUP; 
  GpioCtrlRegs.GPAPUD.bit.GPIO15 = DIS_PULLUP; 
  GpioCtrlRegs.GPAPUD.bit.GPIO16 = DIS_PULLUP; 

  GpioCtrlRegs.GPAMUX1.bit.GPIO14 = MUX_GPIO14_GPIO14;
  GpioCtrlRegs.GPAMUX1.bit.GPIO15 = MUX_GPIO15_GPIO15;
  GpioCtrlRegs.GPAMUX2.bit.GPIO16 = MUX_GPIO16_GPIO16;  

  GpioCtrlRegs.GPAPUD.bit.GPIO17 = DIS_PULLUP; 
  GpioCtrlRegs.GPAPUD.bit.GPIO22 = DIS_PULLUP; 
  GpioCtrlRegs.GPAPUD.bit.GPIO19 = DIS_PULLUP; 

  GpioCtrlRegs.GPAMUX2.bit.GPIO17 = MUX_GPIO17_GPIO17;
  GpioCtrlRegs.GPAMUX2.bit.GPIO22 = MUX_GPIO22_GPIO22;
  GpioCtrlRegs.GPAMUX2.bit.GPIO19 = MUX_GPIO19_GPIO19;  
  EDIS;  

}

////////////////////////////////////////////////////////////////////////////////
/// Return electrical angle from hall sensors. It's a generic function          
////////////////////////////////////////////////////////////////////////////////
int
Hall_angle(unsigned int h1, unsigned int h2, unsigned int h3)
{
  if(h1)
  {
    if(h2)
    {
        return P5+OFFSET;
    }
    else
    {
      if(h3)
      {
        return P3+OFFSET;
      }
      else
      {
        return P4+OFFSET;
      }
    }
  }
  else
  {
    if(h2)
    {
      if(h3)
      {
        return P1+OFFSET;
      }
      else
      {
        return P6+OFFSET;
      }
    }
    else
    {
      return P2+OFFSET;
    }
  } 
} 

////////////////////////////////////////////////////////////////////////////////
/// Return electrical angle from hall sensors for first motor                   
////////////////////////////////////////////////////////////////////////////////
int 
Hall_angle1 (void)
{
	return Hall_angle(H1a,H2a,H3a);
}

////////////////////////////////////////////////////////////////////////////////
/// Return electrical angle from hall sensors for second motor                  
////////////////////////////////////////////////////////////////////////////////
int 
Hall_angle2 (void)
{
	return Hall_angle(H1b,H2b,H3b);
}


