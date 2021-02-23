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
/// @file     pwm.c                                                             
/// @language ASCII/C                                                           
/// @svn      $Id: pwm.c 215 2007-05-01 21:28:50Z ychevall@heig-vd.ch $         
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   
#include "pwm.h"                
#include "regul.h"  

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM1                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm1_InitGpio(void)
{
  EALLOW;  
  GpioCtrlRegs.GPAPUD.bit.GPIO0 = EN_PULLUP; // Enable pull-up on GPIO0 (EPWM1A)
  GpioCtrlRegs.GPAPUD.bit.GPIO1 = EN_PULLUP; // Enable pull-up on GPIO1 (EPWM1B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO0 = MUX_GPIO0_EPWM1A; // Configure GPIO0 as EPWM1A
  GpioCtrlRegs.GPAMUX1.bit.GPIO1 = MUX_GPIO1_EPWM1B; // Configure GPIO1 as EPWM1B
  EDIS;
} 

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM2                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm2_InitGpio(void)
{
  EALLOW;   
  GpioCtrlRegs.GPAPUD.bit.GPIO2 = EN_PULLUP; // Enable pull-up on GPIO2 (EPWM2A)
  GpioCtrlRegs.GPAPUD.bit.GPIO3 = EN_PULLUP; // Enable pull-up on GPIO3 (EPW2B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO2 = MUX_GPIO2_EPWM2A; // Configure GPIO2 as EPWM2A
  GpioCtrlRegs.GPAMUX1.bit.GPIO3 = MUX_GPIO3_EPWM2B; // Configure GPIO3 as EPWM2B
  EDIS;
}   

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM3                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm3_InitGpio(void)
{
  EALLOW;   
  GpioCtrlRegs.GPAPUD.bit.GPIO4 = EN_PULLUP; // Enable pull-up on GPIO4 (EPWM3A)
  GpioCtrlRegs.GPAPUD.bit.GPIO5 = EN_PULLUP; // Enable pull-up on GPIO5 (EPWM3B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO4 = MUX_GPIO4_EPWM3A; // Configure GPIO4 as EPWM3A
  GpioCtrlRegs.GPAMUX1.bit.GPIO5 = MUX_GPIO5_EPWM3B; // Configure GPIO5 as EPWM3B
  EDIS;
}   

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM4                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm4_InitGpio(void)
{
  EALLOW;   
  GpioCtrlRegs.GPAPUD.bit.GPIO6 = EN_PULLUP; // Enable pull-up on GPIO6 (EPWM4A)
  GpioCtrlRegs.GPAPUD.bit.GPIO7 = EN_PULLUP; // Enable pull-up on GPIO7 (EPWM4B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO6 = MUX_GPIO6_EPWM4A; // Configure GPIO6 as EPWM4A
  GpioCtrlRegs.GPAMUX1.bit.GPIO7 = MUX_GPIO7_EPWM4B; // Configure GPIO7 as EPWM4B
  EDIS;
}   

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM5                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm5_InitGpio(void)
{
  EALLOW;   
  GpioCtrlRegs.GPAPUD.bit.GPIO8 = EN_PULLUP; // Enable pull-up on GPIO8 (EPWM5A)
  GpioCtrlRegs.GPAPUD.bit.GPIO9 = EN_PULLUP; // Enable pull-up on GPIO9 (EPWM5B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO8 = MUX_GPIO8_EPWM5A; // Configure GPIO8 as EPWM5A
  GpioCtrlRegs.GPAMUX1.bit.GPIO9 = MUX_GPIO9_EPWM5B; // Configure GPIO9 as EPWM5B
  EDIS;
}   

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM6                                                  
///////////////////////////////////////////////////////////////////////////////
void 
EPwm6_InitGpio(void)
{
  EALLOW;   
  GpioCtrlRegs.GPAPUD.bit.GPIO10 = EN_PULLUP; // Enable pull-up on GPIO10 (EPWM10A)
  GpioCtrlRegs.GPAPUD.bit.GPIO11 = EN_PULLUP; // Enable pull-up on GPIO11 (EPWM11B)
  GpioCtrlRegs.GPAMUX1.bit.GPIO10 = MUX_GPIO10_EPWM6A; // Configure GPIO10 as EPWM6A
  GpioCtrlRegs.GPAMUX1.bit.GPIO11 = MUX_GPIO11_EPWM6B; // Configure GPIO11 as EPWM6B
  EDIS;
}     

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM Trip                                              
///////////////////////////////////////////////////////////////////////////////
void 
EPwm_InitTripGpio(void)
{
  EALLOW;
  GpioCtrlRegs.GPAPUD.bit.GPIO12 = EN_PULLUP;        // Enable pull-up on GPIO12 (TZ1)
  GpioCtrlRegs.GPAQSEL1.bit.GPIO12 = GPXQSELX_ASYNC; // Asynch input GPIO12 (TZ1)
  GpioCtrlRegs.GPAMUX1.bit.GPIO12 = MUX_GPIO12_TZ1;  // Configure GPIO12 as TZ1
  EDIS;   
}                  

///////////////////////////////////////////////////////////////////////////////
/// Initialize Gpio for ePWM                                                   
///////////////////////////////////////////////////////////////////////////////
void
EPwm_InitGpio()
{
  EPwm1_InitGpio(); // Configure PWM 1 for motor A
  EPwm2_InitGpio(); // Configure PWM 2 for motor A
  EPwm3_InitGpio(); // Configure PWM 3 for motor A

  EPwm4_InitGpio(); // Configure PWM 4 for motor B
  EPwm5_InitGpio(); // Configure PWM 5 for motor B
  EPwm6_InitGpio(); // Configure PWM 6 for motor B
   
  EPwm_InitTripGpio(); // Configure TRIP inputs
}

///////////////////////////////////////////////////////////////////////////////
/// Power Off                                                                  
///////////////////////////////////////////////////////////////////////////////
void EPwmA_PowerOff()
{
  EPwmA_Trip();
  EPwm1Regs.CMPA.half.CMPA = 0;
  EPwm2Regs.CMPA.half.CMPA = 0;
  EPwm3Regs.CMPA.half.CMPA = 0;
}

void EPwmB_PowerOff()
{
  EPwmB_Trip();
  EPwm4Regs.CMPA.half.CMPA = 0;
  EPwm5Regs.CMPA.half.CMPA = 0;
  EPwm6Regs.CMPA.half.CMPA = 0;
}

void EPwm_PowerOff()
{    
  EPwm1Regs.ETPS.bit.INTPRD = 0;  
  EPwmA_PowerOff();
  EPwmB_PowerOff();
}

///////////////////////////////////////////////////////////////////////////////
/// Power On                                                                   
///////////////////////////////////////////////////////////////////////////////
void EPwmA_PowerOn()
{
  EPwmA_ClrTrip();
  EPwm1Regs.CMPA.half.CMPA = SP;
  EPwm2Regs.CMPA.half.CMPA = SP;
  EPwm3Regs.CMPA.half.CMPA = SP;
}

void EPwmB_PowerOn()
{
  EPwmB_ClrTrip();
  EPwm4Regs.CMPA.half.CMPA = SP;
  EPwm5Regs.CMPA.half.CMPA = SP;
  EPwm6Regs.CMPA.half.CMPA = SP;
}

void EPwm_PowerOn()
{    
  EPwmA_PowerOn();
  EPwmB_PowerOn();
  EPwm1Regs.ETPS.bit.INTPRD = 1;    
}

///////////////////////////////////////////////////////////////////////////////
/// Configure PWM1 register                                                    
/// For this PWM, the initialization is fully commented just for enjoy the     
/// curious programmer...                                                      
///////////////////////////////////////////////////////////////////////////////
void 
EPwm1_ConfigureRegisters()
{


	/////////////////////////////////////////
  // Counter-Compare Submodule Registers //
	/////////////////////////////////////////
  {
    // Counter-Compare A Register
    EPwm1Regs.CMPA.half.CMPA = 0;  
    
    // Counter-Compare B Register
    EPwm1Regs.CMPB = ADC_SOC;         // Used for ADC start-of-conversion
  }

	//////////////////////////////////////////
  // Action-Qualifier Submodule Registers //
	//////////////////////////////////////////
  {
    // Action-Qualifier Output A Control Register
    EPwm1Regs.AQCTLA.bit.CAU = 1;     // Output high when the counter is incrementing
    EPwm1Regs.AQCTLA.bit.CAD = 2;     // Output low when the counter is decrementing
  }

	/////////////////////////////////////////
  // Death-Band Submodule Registers      //
	/////////////////////////////////////////
  {
    // Death-Band Generator Control Register
    EPwm1Regs.DBCTL.bit.POLSEL   = 1;  
    EPwm1Regs.DBCTL.bit.OUT_MODE = 3;
    EPwm1Regs.DBCTL.bit.IN_MODE  = 0;

    // Death-Band Generator Rising Edge Delay Register
    EPwm1Regs.DBRED = DT;               // Risingg Edge Delay Count (10 bit counter)

    // Death-Band Generator Falling Edge Delay Register
    EPwm1Regs.DBFED = DT;               // Falling Edge Delay Count (10 bit counter)
  }

  ////////////////////////////////////////////
  // PWM-Chopper Submodule Control Register //
  ////////////////////////////////////////////
  {
    // PWM-Chopper Control Register
    EPwm1Regs.PCCTL.bit.CHPEN = 0;      // PWM-Chopper is disabled
  }

  //////////////////////////////////////////////////////
  // Trip-Zone Submodule Control and Status Registers //
  //////////////////////////////////////////////////////
  {
    EALLOW; 

    // Trip-Zone Select Register
    //EPwm1Regs.TZSEL.bit.OSHT1 = 1;     // Enable TZ1 as a one-shot trip source

    // Trip-Zone Control Register
    EPwm1Regs.TZCTL.bit.TZA = 1;       // Force EPWMxA to a low state if trip event occurs
    EPwm1Regs.TZCTL.bit.TZB = 1;       // Force EPWMxB to a low state if trip event occurs

    // Trip-Zone Enable Interupt Register
    EPwm1Regs.TZEINT.bit.OST = 0;      // Disable one-shot interrupt generation
    EPwm1Regs.TZEINT.bit.CBC = 0;      // Disable cycle-by-cycle interrupt generation

    // Trip-Zone Clear Register
    EPwm1Regs.TZCLR.bit.CBC = 1;       // Clear this Trip condition
    EPwm1Regs.TZCLR.bit.OST = 1;       // Clear this Trip condition
    EPwm1Regs.TZCLR.bit.INT = 1;       // Clear this Trip-interrupt flag

    EDIS; 
  }

  /////////////////////////////////////////
  // Time-Base Configuration             //
  /////////////////////////////////////////
  {
    // Time-Base Control Register
    EPwm1Regs.TBCTL.bit.PHSEN = 0;    // Master module
    EPwm1Regs.TBCTL.bit.PHSDIR = 0;   // Count down after the synchronization event
    EPwm1Regs.TBCTL.bit.SYNCOSEL = 1; // Synchro. Out. when CTR = zero 
    EPwm1Regs.TBCTL.bit.PRDLD = 1;    // Directly accesses to the TBPRD active Register
    EPwm1Regs.TBCTL.bit.CTRMODE = 2;  // Up-down count mode

    // Time-Base Status Register
    EPwm1Regs.TBSTS.bit.CTRMAX = 1;   // Clear the latched event
    EPwm1Regs.TBSTS.bit.SYNCI = 1;    // Clear the latched event

    // Time-Base Period Register
    EPwm1Regs.TBPRD = SP; 
   
    // Time-Base Phase Register
    EPwm1Regs.TBPHS.half.TBPHS = 0; 

    // Time-Base Counter Register
    EPwm1Regs.TBCTR = 0; 
  }

	/////////////////////////////////////////
  // Event-Trigger Submodule Registers   //
	/////////////////////////////////////////
  {
    // Event-Trigger Prescale Register
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;    // Generate SOCA event on the first event ETPS
    EPwm1Regs.ETPS.bit.INTPRD = 1;     // Generate interrupt on the first event

    // Event-Trigger Clear Register
    EPwm1Regs.ETCLR.bit.SOCA = 1;      // Clear ETFLG[SOCA] flag bit
    EPwm1Regs.ETCLR.bit.SOCB = 1;      // Clear ETFLG[SOCB] flag bit
    EPwm1Regs.ETCLR.bit.INT = 1;       // Clear ETFLG[INT] flag bit

    // Event-Trigger Force Register
    EPwm1Regs.ETFRC.all = 0; // Rajout  

    // Event-Trigger Flag Register
    EPwm1Regs.ETFLG.all = 0;  // Rajout

    // Event-Trigger Selection Register
    EPwm1Regs.ETSEL.bit.SOCASEL = 7;   // Enable event: time-base counter equal to CMPB when the timer isdecrementing
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;    // Enable EPWMxSOCA pulse

    EPwm1Regs.ETSEL.bit.INTSEL = 1;    // Enable event time-base counter equal to zero (TBCTR=0x0000)
    EPwm1Regs.ETSEL.bit.INTEN = 1;     // Enable EPWMx_INT generation
  }
}

///////////////////////////////////////////////////////////////////////////////
/// Common initialization for slave PWM channels                               
///////////////////////////////////////////////////////////////////////////////
void 
EPwmSlave_ConfigureRegisters(volatile struct EPWM_REGS * EPwmSlaveRegs )
{
  
  EPwmSlaveRegs->CMPA.half.CMPA = 0;       // 50% Duty Cycle
  EPwmSlaveRegs->CMPB = 0;                 // Not-used
  EPwmSlaveRegs->AQCTLA.bit.CAU = 1;       // Output high when the counter is incrementing
  EPwmSlaveRegs->AQCTLA.bit.CAD = 2;       // Output low when the counter is decrementing
  EPwmSlaveRegs->DBCTL.bit.POLSEL   = 1;  
  EPwmSlaveRegs->DBCTL.bit.OUT_MODE = 3;
  EPwmSlaveRegs->DBCTL.bit.IN_MODE  = 0;
  EPwmSlaveRegs->DBRED = DT;               // Risingg Edge Delay Count (10 bit counter)
  EPwmSlaveRegs->DBFED = DT;               // Falling Edge Delay Count (10 bit counter)
  EPwmSlaveRegs->PCCTL.bit.CHPEN = 0;      // PWM-Chopper is disabled

  EALLOW;
  //EPwmSlaveRegs->TZSEL.bit.OSHT1 = 1;      // Enable TZ1 as a one-shot trip source
  EPwmSlaveRegs->TZCTL.bit.TZA = 1;        // Force EPWMxA to a low state if trip event occurs
  EPwmSlaveRegs->TZCTL.bit.TZB = 1;        // Force EPWMxB to a low state if trip event occurs
  EDIS;

  EPwmSlaveRegs->TZEINT.bit.OST = 0;       // Disable one-shot interrupt generation
  EPwmSlaveRegs->TZEINT.bit.CBC = 0;       // Disable cycle-by-cycle interrupt generation
  EPwmSlaveRegs->TZCLR.bit.CBC = 1;        // Clear this Trip condition
  EPwmSlaveRegs->TZCLR.bit.OST = 1;        // Clear this Trip condition
  EPwmSlaveRegs->TZCLR.bit.INT = 1;        // Clear this Trip-interrupt flag
  EPwmSlaveRegs->TBCTL.bit.PHSEN = 1;      // Slave module
  EPwmSlaveRegs->TBCTL.bit.PHSDIR = 1;     // Count down after the synchronization event
  EPwmSlaveRegs->TBCTL.bit.SYNCOSEL = 0;   // Sync flow-through
  EPwmSlaveRegs->TBCTL.bit.PRDLD = 1;      // Directly accesses to the TBPRD active Register
  EPwmSlaveRegs->TBCTL.bit.CTRMODE = 2;    // Up-down count mode
  EPwmSlaveRegs->TBSTS.bit.CTRMAX = 1;     // Clear the latched event
  EPwmSlaveRegs->TBSTS.bit.SYNCI = 1;      // Clear the latched event
  EPwmSlaveRegs->TBPRD = SP; 
  EPwmSlaveRegs->TBPHS.half.TBPHS = 0; 
  EPwmSlaveRegs->TBCTR = 0; 
  EPwmSlaveRegs->ETFRC.all = 0;           // Rajout  
  EPwmSlaveRegs->ETFLG.all = 0;           // Rajout
}   

///////////////////////////////////////////////////////////////////////////////
/// Initialize Registers for ePWM                                              
///////////////////////////////////////////////////////////////////////////////
void
EPwm_InitRegisters()
{
    EPwm1_ConfigureRegisters();								// Master Module
	EPwmSlave_ConfigureRegisters(&EPwm2Regs);  // Slave Module
	EPwmSlave_ConfigureRegisters(&EPwm3Regs);  // Slave Module
	EPwmSlave_ConfigureRegisters(&EPwm4Regs);  // Slave Module
	EPwmSlave_ConfigureRegisters(&EPwm5Regs);  // Slave Module
	EPwmSlave_ConfigureRegisters(&EPwm6Regs);  // Slave Module
}

///////////////////////////////////////////////////////////////////////////////
/// Initialize PWM Controller                                                  
///////////////////////////////////////////////////////////////////////////////
void 
InitEPwm (void)
{
  EPwm_InitRegisters();  // Very Important Remark: This Initialisation must be before GPIO init. 
  EPwm_InitGpio();

  EALLOW;
  SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC =1;
  EDIS;  
}

///////////////////////////////////////////////////////////////////////////////
/// Set PWM 1,2,3 Duty Cycle in 1.15 format.                                   
///////////////////////////////////////////////////////////////////////////////
void 
EPwm_MotorA(three_phases u)
{
  // u and w are crossed for hardware specific connections
  EPwm1Regs.CMPA.half.CMPA = _IQmpy((_iq)u.w+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
  EPwm2Regs.CMPA.half.CMPA = _IQmpy((_iq)u.v+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
  EPwm3Regs.CMPA.half.CMPA = _IQmpy((_iq)u.u+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
}

///////////////////////////////////////////////////////////////////////////////
/// Set PWM 4,5,6 Duty Cycle in 1.15 format.                                   
///////////////////////////////////////////////////////////////////////////////
void 
EPwm_MotorB(three_phases u)
{
  // u and w are crossed for respect specific connections
  EPwm4Regs.CMPA.half.CMPA = _IQmpy((_iq)u.w+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
  EPwm5Regs.CMPA.half.CMPA = _IQmpy((_iq)u.v+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
  EPwm6Regs.CMPA.half.CMPA = _IQmpy((_iq)u.u+0x8000, (_iq)(HALF_SP-2*SAT_SP))+SAT_SP;
}

///////////////////////////////////////////////////////////////////////////////
/// Trip motor A                                                               
///////////////////////////////////////////////////////////////////////////////
void 
EPwmA_Trip()
{
  EALLOW;
  EPwm1Regs.TZFRC.bit.OST = 1; 
  EPwm2Regs.TZFRC.bit.OST = 1; 
  EPwm3Regs.TZFRC.bit.OST = 1; 
  EDIS; 
}

///////////////////////////////////////////////////////////////////////////////
/// Trip motor B                                                               
///////////////////////////////////////////////////////////////////////////////
void 
EPwmB_Trip()
{

  EALLOW;
  EPwm4Regs.TZFRC.bit.OST = 1; 
  EPwm5Regs.TZFRC.bit.OST = 1; 
  EPwm6Regs.TZFRC.bit.OST = 1; 
  EDIS; 
}

///////////////////////////////////////////////////////////////////////////////
/// Trip motor A                                                               
///////////////////////////////////////////////////////////////////////////////
void 
EPwmA_ClrTrip()
{

  EALLOW;
  EPwm1Regs.TZCLR.bit.OST = 1; 
  EPwm2Regs.TZCLR.bit.OST = 1; 
  EPwm3Regs.TZCLR.bit.OST = 1; 
  EDIS; 
}

///////////////////////////////////////////////////////////////////////////////
/// Trip motor B                                                               
///////////////////////////////////////////////////////////////////////////////
void 
EPwmB_ClrTrip()
{
  EALLOW;
  EPwm4Regs.TZCLR.bit.OST = 1; 
  EPwm5Regs.TZCLR.bit.OST = 1; 
  EPwm6Regs.TZCLR.bit.OST = 1; 
  EDIS; 
}


////////////////////////////////////////////////////////////////////////////////
/// End of file.                                                                
////////////////////////////////////////////////////////////////////////////////

