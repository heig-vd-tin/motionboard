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
/// @file     common.h                                                          
/// @language ASCII/C                                                           
/// @svn      $Id: common.h 173 2007-03-31 22:49:25Z ychevall@heig-vd.ch $                     
///                                                                             
////////////////////////////////////////////////////////////////////////////////
#ifndef __COMMON__
#define __COMMON__

////////////////////////////////////////////////////////////////////////////////
/// Specify the PLL control register (PLLCR) and clock in divide (CLKINDIV) val.
/// if CLKINDIV = 0: SYSCLKOUT = (OSCCLK * PLLCR)/2                             
/// if CLKINDIV = 1: SYSCLKOUT = (OSCCLK * PLLCR)                               
////////////////////////////////////////////////////////////////////////////////
#define DSP28_CLKINDIV  0

#define DSP28_PLLCR    10   
//#define DSP28_PLLCR   9
//#define DSP28_PLLCR   8
//#define DSP28_PLLCR   7
//#define DSP28_PLLCR   6
//#define DSP28_PLLCR   5
//#define DSP28_PLLCR   4
//#define DSP28_PLLCR   3
//#define DSP28_PLLCR   2
//#define DSP28_PLLCR   1
//#define DSP28_PLLCR   0 // PLL is bypassed in this mode

////////////////////////////////////////////////////////////////////////////////
/// Specify the clock rate of the CPU (SYSCLKOUT) in nS.                        
/// Take into account the input clock frequency and the PLL multiplier          
/// selected in step 1.                                                         
///                                                                             
/// Use one of the values provided, or define your own. The trailing L is       
/// required tells the compiler to treat the number as a 64-bit value.          
///                                                                             
/// Only one statement should be uncommented.                                   
/// Example:  CLKIN is a 20MHz crystal.                                         
///                                                                             
///           In step 1 the user specified PLLCR = 0xA for a                    
///           100Mhz CPU clock (SYSCLKOUT = 100MHz).                            
///                                                                             
///           In this case, the CPU_RATE will be 10.000L                        
///           Uncomment the line:  #define CPU_RATE   10.000L                   
////////////////////////////////////////////////////////////////////////////////
#define CPU_RATE     10.000L   // for a 100MHz CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   13.330L   // for a 75MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   16.667L   // for a 60MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   20.000L   // for a 50MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   33.333L   // for a 30MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   41.667L   // for a 24MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   50.000L   // for a 20MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE   66.667L   // for a 15MHz  CPU clock speed  (SYSCLKOUT)
//#define CPU_RATE  100.000L   // for a 10MHz  CPU clock speed  (SYSCLKOUT)

////////////////////////////////////////////////////////////////////////////////
/// Define SYSCLKOUT constant                                                   
////////////////////////////////////////////////////////////////////////////////
#if DSP28_CLKINDIV == 0 
	#define SYSCLKOUT (CPU_RATE*2) 
#else
	#define SYSCLKOUT (CPU_RATE)
#endif

////////////////////////////////////////////////////////////////////////////////
/// Include Global Prototyping header file                                      
////////////////////////////////////////////////////////////////////////////////
#include "DSP280x_GlobalPrototypes.h"       

////////////////////////////////////////////////////////////////////////////////
/// Header file define Default ISR is used only in debug mode                   
////////////////////////////////////////////////////////////////////////////////
#ifndef DSP28_BIOS
#include "DSP280x_DefaultISR.h"
#endif

////////////////////////////////////////////////////////////////////////////////
/// Somes macros used in somes others files                                     
////////////////////////////////////////////////////////////////////////////////
#define DELAY_US(A)  DSP28x_usDelay(((((long double) A * 1000.0L) / (long double)CPU_RATE) - 9.0L) / 5.0L)
#define REMAP_INTERRUPT(A,B) EALLOW; PieVectTable.A = B; EDIS

#endif
////////////////////////////////////////////////////////////////////////////////
/// End of file...                                                              
////////////////////////////////////////////////////////////////////////////////

