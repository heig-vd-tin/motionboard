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
/// @file     eqep.c                                                            
/// @language ASCII/C                                                           
/// @svn      $Id: eqep.c 324 2008-01-21 10:51:56Z ychevall@heig-vd.ch $        
///                                                                             
////////////////////////////////////////////////////////////////////////////////
/// Description.                                                                
/// Le module EQep intégré dans la famille des DSP texas 320F280x permet de     
/// traiter les signaux reçus par des codeurs de position avec signaux en       
/// quadrature. Ce fichier contient les routines d'initialisation de ce périph- 
/// érique ainsi que les routines permettant le calcul de la position, des      
/// angles mechanique et électrique ainsi que la vitesse. Un méchanisme         
/// d'interpolation permet d'obtenir une plus grande résolution sur la vitesse. 
////////////////////////////////////////////////////////////////////////////////
#include "device.h"    
#include "common.h"  
#include "IQmathLib.h"
#include "hall.h"  
#include "eqep.h"
#include "qmath.h" 

////////////////////////////////////////////////////////////////////////////////
/// Ram Execution...                                                            
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SECTION(EQep_process, "ramfuncs");
#pragma CODE_SECTION(EQep_velocity, "ramfuncs");

////////////////////////////////////////////////////////////////////////////////
/// Initialize GPIO for EQep1.                                                  
////////////////////////////////////////////////////////////////////////////////
void EQep1_InitGpio()
{
  EALLOW;
  GpioCtrlRegs.GPAMUX2.bit.GPIO20 = MUX_GPIO20_EQEP1A; 
  GpioCtrlRegs.GPAMUX2.bit.GPIO21 = MUX_GPIO21_EQEP1B;  
  GpioCtrlRegs.GPAMUX2.bit.GPIO23 = MUX_GPIO23_EQEP1I; 
  EDIS;  
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize GPIO for EQep2.                                                  
////////////////////////////////////////////////////////////////////////////////
void EQep2_InitGpio()
{
  EALLOW;
  GpioCtrlRegs.GPAMUX2.bit.GPIO24 = MUX_GPIO24_EQEP2A;  
  GpioCtrlRegs.GPAMUX2.bit.GPIO25 = MUX_GPIO25_EQEP2B; 
  GpioCtrlRegs.GPAMUX2.bit.GPIO26 = MUX_GPIO26_EQEP2I;  
  EDIS;  
}   

////////////////////////////////////////////////////////////////////////////////
/// Initialize GPIO for both EQep modules                                       
////////////////////////////////////////////////////////////////////////////////
void
EQep_InitGpio()
{
	EQep1_InitGpio();
	EQep2_InitGpio();
}

////////////////////////////////////////////////////////////////////////////////
/// Common initialisation for all EQep modules                                  
/// Le module est tout d'abord désactivé avant d'être configuré. Ceci permet d' 
/// éviter des problèmes lors des phases de debug. Le module est ensuite        
/// configuré en mode Quadrature. Les signaux A et B sont interprêté dans une   
/// Machine d'état pour fournir une information de direction et de position     
/// Ce mode permet d'accroitre la précision du codeur par un facteur 4. Le      
/// compteur de position (sur 32 bits) n'est pas remis à zero automatiquement   
/// En revanche lorsque le signal d'index est détecté, la position courante est 
/// copiée dans le registre QPOSILAT. Lorsque ce registre est lu par le soft,   
/// les valeurs de QCTMR et QCPRD sont également latchées. Elles permettent l'  
/// interpolation de la vitesse permettant à celle-ci d'être moins granuleuse   
/// Ceci dans le but d'accroitre au maximuk le gain de la boucle de vitesse qui 
/// doit être le plus élevé possible pour maximiser les performances de la      
/// boucle de position.                                                         
////////////////////////////////////////////////////////////////////////////////
void 
EQep_InitCommonRegisters(volatile struct EQEP_REGS *EQepRegsP)
{
  volatile struct EQEP_REGS *EQepRegs; 

  EQepRegs = EQepRegsP; 

  EQepRegs->QEPCTL.bit.QPEN=0; 	  	// QEP enable
  EQepRegs->QCAPCTL.bit.CEN=0;      // QEP Capture Enable

  EQepRegs->QDECCTL.bit.QSRC=0;	// Quadrature mode enabled
  EQepRegs->QEPCTL.bit.FREE_SOFT=2; // Position counter is unaffected by emulation suspend
  EQepRegs->QEPCTL.bit.PCRM=1;	    // Position counter reset on maximum position
  EQepRegs->QEPCTL.bit.QCLM=0; 	    // Latch QCTMRLAT and QCPRDLAT on QPOSCNT reading
  EQepRegs->QEPCTL.bit.IEL=1;      // Latch Position on index event

  EQepRegs->QPOSMAX=0xFFFFFFFF;     // QPOSMAX receive maximum value as possible
  EQepRegs->QEPCTL.bit.QPEN=1; 	  	// QEP enable

  EQepRegs->QCAPCTL.bit.UPPS=1;   	// 1/1 for unit position
  EQepRegs->QCAPCTL.bit.CCPS=1;	    // 1/1 for CAP clock
  EQepRegs->QCAPCTL.bit.CEN=1; 	    // QEP Capture Enable

  EQepRegs->QPOSCNT = 0; 			// Reset position counter
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize both EQep                                                        
////////////////////////////////////////////////////////////////////////////////
void 
EQep_InitRegisters()
{
  EQep_InitCommonRegisters(&EQep1Regs);
  EQep_InitCommonRegisters(&EQep2Regs);
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize EQep1                                                            
////////////////////////////////////////////////////////////////////////////////
void 
InitEQep ()
{
  EQep_InitRegisters();
  EQep_InitGpio();
}

////////////////////////////////////////////////////////////////////////////////
/// EQep Zero Position                                                         
////////////////////////////////////////////////////////////////////////////////
void
EQep_set_zero(eqep_handle p)
{
  volatile struct EQEP_REGS *EQepRegs; // Thanks to MSL for this good trick

  // Save EQep pointer in a local pointer
  EQepRegs = p->EQepRegs;

  // Reinit position
  p->position = 0; 
  p->revolution = 0; 
  p->oldposition = EQepRegs->QPOSCNT; 
  p->oldposition2 = 0; 
  p->velocity = 0;
  p->delta_pos = 0; 
}

////////////////////////////////////////////////////////////////////////////////
/// EQep Position Measurement                                                   
////////////////////////////////////////////////////////////////////////////////
#define oldpos p->oldposition
#define middlecount (EQepRegs->QPOSMAX/2)
void
EQep_process(eqep_handle p)
{
  volatile struct EQEP_REGS *EQepRegs; // Thanks to MSL for this good trick
  volatile unsigned long curpos; 

  // Save EQep pointer in a local pointer
  EQepRegs = p->EQepRegs; 

  // What is direction ?
  p->direction  = EQepRegs->QEPSTS.bit.QDF;

  // Calculate mechanical and electrical angle in 1.15 format
  p->theta_raw = EQepRegs->QPOSCNT - EQepRegs->QPOSILAT; 
  p->theta_mech = _IQmpy(p->theta_raw, _IQ(32768/4096));
  p->theta_elec = (p->theta_mech*p->pole_pairs*2) + p->cal_angle;

  // Count revolution and Check an index occurence
  if (EQepRegs->QFLG.bit.IEL == 1) 
  {
    EQepRegs->QCLR.bit.IEL = 1;
    p->index_sync_flag = 1;
    p->revolution += (p->direction)?1:-1;  
  }

  // Check an index occurence 
  if (p->sine == 0 || p->index_sync_flag == 0)
    p->theta_elec = p->HallFunction(); 

  // Compute Trigonometric values
  p->theta_sincos.sin = qsin(p->theta_elec);
  p->theta_sincos.cos = qcos(p->theta_elec);

  // Compute delta position
  curpos = EQepRegs->QPOSCNT; 
  if(oldpos > curpos && oldpos > middlecount && curpos < middlecount)
    p->delta_pos = EQepRegs->QPOSMAX - oldpos + curpos;
  else if(oldpos > curpos)
    p->delta_pos = -(oldpos - curpos);
  else if(curpos > middlecount && oldpos < middlecount)
    p->delta_pos = -(EQepRegs->QPOSMAX - curpos + oldpos);
  else 
    p->delta_pos = curpos - oldpos; 

  p->oldposition = curpos; 

  // Compute position 
  p->position += p->delta_pos; 

}

////////////////////////////////////////////////////////////////////////////////
/// EQep Velocity Measurement                                                   
////////////////////////////////////////////////////////////////////////////////
void
EQep_velocity (eqep_handle p)
{
  #define SPEED_SCALE 273
  volatile struct EQEP_REGS *EQepRegs;
  volatile long curpos;  
  int velo = 0; 
  int veloFine = 0;   

  // Save EQep pointer in a local pointer
  EQepRegs = p->EQepRegs; 
    
  // Compute position directly from coder ticks
  curpos = p->position;
  velo = (curpos - p->oldposition2) * SPEED_SCALE;
  p->oldposition2 = curpos;
  
  // Compte fine position from EQep p
  if(EQepRegs->QEPSTS.bit.COEF == 0)
    veloFine = _IQmpy(_IQdiv(EQepRegs->QCTMRLAT, EQepRegs->QCPRDLAT), _IQ(0.00366)); 
  else
    veloFine = SPEED_SCALE;

  // If direction has changed fine position can't be computed
  if(EQepRegs->QEPSTS.bit.CDEF == 1)
    veloFine = 0; 

  // The final position is base position plus fine position
  p->velocity = velo + veloFine; 

  // Finally the event clag is cleared
  EQepRegs->QEPSTS.all=0x88;			
}

////////////////////////////////////////////////////////////////////////////////
/// EQep XY measurement by odometry theorie                                     
////////////////////////////////////////////////////////////////////////////////
#define entrax 300
#define inv_entrax (_IQ(0.000153))

_iq cap = 0;  

volatile long long oldpositionl = 0, oldpositionr = 0; 

void
EQep_xy (eqep_handle pl, eqep_handle pr, long long *x, long long *y)
{
/*
  const _iq f_alpha = _IQ30(0.0000223349679); 
  const _iq f_delta = 
  _iq dl, dr, dx, dy; 
  _iq d_alpha, d_delta; 
//  _iq err_alpha; 
 // _iq d_norm_comp_alpha = 0, d_norm_alpha = 0, cum_err_alpha = 0; 


  // Compute delta position (_IQ25)
  dl = (pl->position - oldpositionl)<<25; 
  dr = (pr->position - oldpositionr)<<25; 
  oldpositionl = pl->position; 
  oldpositionr = pr->position;

  // Compute delta mvt and delta revolution (_IQ25)
  d_alpha = (dl+dr)/2;
  d_delta = (dl-dr)/2;

  // Normalise l'angle fonction des paramètres du robot
  d_norm_alpha = _IQ20mpy(d_alpha<<5, entrax);

  err_alpha = d_alpha - _IQ20mpy(d_norm_alpha, entrax); 
  cum_err_alpha += err_alpha;  
  d_norm_comp_alpha += d_norm_alpha + cum_err_alpha;


  cap += d_alpha; 



  dx = _IQmpy(_IQ29cos(d_alpha), d_delta); 
  dy = _IQmpy(_IQ29sin(d_alpha), d_delta); 

  *x += (long long)dx; 
  *y += (long long)dy;*/
}

////////////////////////////////////////////////////////////////////////////////
/// Speed & Position Calculation.                                               
////////////////////////////////////////////////////////////////////////////////

