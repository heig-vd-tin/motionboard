#ifndef __PWM__
#define __PWM__ 

#include "regul.h"  
#include "common.h"   // DSP280x Examples Include File

////////////////////////////////////////////////////////////////////////////////
/// Global constants (Configuration PWM modules)                                
////////////////////////////////////////////////////////////////////////////////
#define CPU_CLK   100e6             // CPU Speed (default: 100MHz)
#define PWM_CLK   24e3              // 24 kHz

#define SP   (int)((long)1e9/(long)PWM_CLK/(long)SYSCLKOUT/2) // Divided by 2 because PWM is in up-down mode
#define HALF_SP (SP/2) 

#define DT     60                   // Death time (1us)
#define SAT_SP 20                   // Border limit Duty Cycle

#define ADC_CONV_SPEED  50   // 50 ns per conversion
#define ADC_NCONV       7    // 7 convertion per sequence

#define ADC_SOC 40; 

////////////////////////////////////////////////////////////////////////////////
/// Function prototypes                                                         
////////////////////////////////////////////////////////////////////////////////
void InitEPwm(void);

void EPwm_MotorA(three_phases u);
void EPwm_MotorB(three_phases u);

void EPwmA_PowerOn();
void EPwmB_PowerOn();
void EPwm_PowerOn();

void EPwmA_PowerOff();
void EPwmB_PowerOff();
void EPwm_PowerOff();

void EPwmA_Trip();
void EPwmB_Trip();

void EPwmA_ClrTrip();
void EPwmB_ClrTrip();

////////////////////////////////////////////////////////////////////////////////
/// Macros definitions                                                          
////////////////////////////////////////////////////////////////////////////////
#define PWM_A_U EPwm1Regs.CMPA.all
#define PWM_A_V EPwm2Regs.CMPA.all
#define PWM_A_W EPwm3Regs.CMPA.all

#define PWM_B_U EPwm4Regs.CMPA.all
#define PWM_B_V EPwm5Regs.CMPA.all
#define PWM_B_W EPwm6Regs.CMPA.all

#endif

