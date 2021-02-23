#ifndef __PWM__
#define __PWM__ 

////////////////////////////////////////////////////////////////////////////////
/// Global constants (Configuration PWM modules)                                
////////////////////////////////////////////////////////////////////////////////
#define SAT_SP 20            // Border limit Duty Cycle

#define ADC_CONV_SPEED  50   // 50 ns per conversion
#define ADC_NCONV       7    // 7 convertion per sequence

#define ADC_SOC 15; 

////////////////////////////////////////////////////////////////////////////////
/// Function prototypes                                                         
////////////////////////////////////////////////////////////////////////////////
void Init_epwm(void);

void epwm_motor_a(float u, float v, float w);
void epwm_motor_b(float u, float v, float w);

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

void EPwm_Frequency(float frequency); 

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

