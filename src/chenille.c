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
/// @file     chenille.c                                                        
/// @language ASCII/C                                                           
/// @svn      $Id: main.c 175 2007-04-01 08:25:38Z ychevall@heig-vd.ch $        
///                                                                             
////////////////////////////////////////////////////////////////////////////////
       
////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   

////////////////////////////////////////////////////////////////////////////////
/// Chenille                                                                    
////////////////////////////////////////////////////////////////////////////////
int chenille_cycle = 0; 

interrupt void
cpu_timer0_isr()
{
  static char i = 0; 
  static int cycle = 0; 
  char seq[4] = {1,2,4,2};

  Leds_set(seq[i++]); 
  if (i >= 4) 
  {
    i = 0;  
    cycle++;
  }
  if(cycle > chenille_cycle) 
  {
    cycle = 0; 
    PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
  }
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

void
initChenille()
{
  REMAP_INTERRUPT(TINT0, &cpu_timer0_isr);   
  InitCpuTimers();   
  ConfigCpuTimer(&CpuTimer0, 100, 100000); // Interrupt is raised every 100 [ms]
  StartCpuTimer0();   
  IER |= M_INT1;
  PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
}

void
chenille_stop()
{
  PieCtrlRegs.PIEIER1.bit.INTx7 = 0;
}

void
chenille_start(int cycle)
{
  chenille_cycle = cycle; 
  PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
}

void
chenille()
{
  Leds_set(0);   
  initChenille(); 
  chenille_start(2);
  while(PieCtrlRegs.PIEIER1.bit.INTx7 == 1);
  Leds_set(0);
  Leds_red(1);
}

