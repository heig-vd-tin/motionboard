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
/// @file     main.c                                                            
/// @language ASCII/C                                                           
/// @svn      $Id: main.c 324 2008-01-21 10:51:56Z ychevall@heig-vd.ch $        
///                                                                             
////////////////////////////////////////////////////////////////////////////////
       
////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   
#include "IQmathLib.h"

#include "leds.h"              
#include "adc.h"               
#include "pwm.h"               
#include "eqep.h"              
#include "fan.h"   
#include "hall.h"            
#include "regul.h" 
#include "i2c.h"
#include "bangbang.h"
#include "memory.h" 
#include "chenille.h"

////////////////////////////////////////////////////////////////////////////////
/// Global constants                                                            
////////////////////////////////////////////////////////////////////////////////
#define LO_VDC_LIMIT 10U //[V]
#define HI_VDC_LIMIT 48U //[V]

////////////////////////////////////////////////////////////////////////////////
/// Ram Execution...                                                            
////////////////////////////////////////////////////////////////////////////////
#pragma CODE_SECTION(pwmSyncIsr, "ramfuncs");
#pragma CODE_SECTION(adcIsr, "ramfuncs");

////////////////////////////////////////////////////////////////////////////////
/// Global variables                                                            
////////////////////////////////////////////////////////////////////////////////

// Global Configuration
volatile globalConfiguration global; 
#define SYNC_INTERVAL 6  // 24e3/6 = 4kHz
#define XY_INTERVAL 50   // Compute XY position every interval * pwmsync
// EQep
eqep motorA = EQEP2_DEFAULTS;
eqep motorB = EQEP1_DEFAULTS;

// BangBang
handle_bangbang bangbangA = BANGBANG_DEFAULTS;
handle_bangbang bangbangB = BANGBANG_DEFAULTS;    

// Analog to Digital Converter
adc  adcResults = ADC_DEFAULTS; 

// Local Variables
three_phases   utria, utrib; 
vectors_phases uvect; 

int moche; 
////////////////////////////////////////////////////////////////////////////////
/// Prototypes                                                                  
////////////////////////////////////////////////////////////////////////////////
void PowerOn();
void PowerOff();
void MotorA_reset(); 
void MotorB_reset(); 

three_phases TorqueController(three_phases, tri_theta, int, bldc_controller*);

int64 pos_yaw = 0; 
int64 pos_x = 0; 

////////////////////////////////////////////////////////////////////////////////
/// PWM ISR                                                                     
////////////////////////////////////////////////////////////////////////////////
interrupt 
void pwmSyncIsr(void)
{
  static int syncCount = 0; // , xyCount = 0;
  static int oldrevA = 0, oldrevB = 0; 
  static int old_ctrl_type_A, old_ctrl_type_B; 

  // Set Green Led for debug information
  #ifndef RELEASE
  Leds_green(1); 
  #endif 

  // Compute current position and current velocity
  EQep_process (&motorA);
  EQep_process (&motorB);

  // Compute XY position
  /*
  if(xyCount++ > XY_INTERVAL) 
  {
	xyCount = 0; 
	EQep_xy (&motorA, &motorB, &(global.xyp.x), &(global.xyp.y)); 
  }*/

  // On Change Mode reset Motors
  if(old_ctrl_type_A != global.mca.ctrl_type) 
  {
    old_ctrl_type_A = global.mca.ctrl_type;    
    MotorA_reset(); 
  }
  if(old_ctrl_type_B != global.mcb.ctrl_type) 
  {
    old_ctrl_type_B = global.mcb.ctrl_type;    
    MotorB_reset(); 
  }

  // Velocity and Position Controller
  if(++syncCount >= SYNC_INTERVAL)
  {
    // Reset SyncCount counter
    syncCount = 0; 

    // Compute motor velocity
    EQep_velocity(&motorA); 
    EQep_velocity(&motorB); 


	if(global.env.sync == 1)
	{
		global.mva.sync = 1; 
		global.mvb.sync = 1;
		global.env.sync = 0;
	}

    // Two axes synchronisation
    if(global.mva.sync == 1)
    {      
	  global.mva.sync = 0; 
      bangbang_mvt (&bangbangA, 
                    global.bba.acceleration, 
                    global.bba.deceleration, 
                    global.bba.velocity, 
                    global.bba.position);
	}
	if(global.mvb.sync == 1)
	{
	  global.mvb.sync = 0;
      bangbang_mvt (&bangbangB, 
                    global.bbb.acceleration, 
                    global.bbb.deceleration, 
                    global.bbb.velocity, 
                    global.bbb.position);
    }

	// BangBang Smooth Stop Required ?
	if(global.bba.stop == 1)
	{
		global.bba.stop = 0;
		bangbang_smooth_stop(&bangbangA);
	}
	if(global.bbb.stop == 1)
	{
		global.bbb.stop = 0;
		bangbang_smooth_stop(&bangbangB);
	}

    // BangBang Routine (Alpha/Delta mode)
    if(global.mca.ctrl_type == BANGBANG && global.mcb.ctrl_type == BANGBANG)
    {
      pos_x   = bangbang_process (&bangbangA, pos_x);
      pos_yaw = bangbang_process (&bangbangB, pos_yaw);

      global.mvb.position = pos_yaw - pos_x; 
      global.mva.position = pos_yaw + pos_x; 
    }

	
    // Velocity and Position Controller
    if(global.mca.ctrl_type >= POSITION)
      global.mva.velocity = RegulPi_p(motorA.position, global.mva.position, &bldc_a); 
    if(global.mca.ctrl_type >= VELOCITY)
      global.mva.torque = RegulPi_v(motorA.velocity, global.mva.velocity, &bldc_a); 

    if(global.mcb.ctrl_type >= POSITION)
      global.mvb.velocity = RegulPi_p(motorB.position, global.mvb.position, &bldc_b); 
    if(global.mcb.ctrl_type >= VELOCITY)
      global.mvb.torque  = RegulPi_v(motorB.velocity, global.mvb.velocity, &bldc_b); 	  
  }

  
  // Low or High Vdc detect
  if((unsigned int)adcResults.dcVoltage < (LO_VDC_LIMIT*1086U) ||
	 (unsigned int)adcResults.dcVoltage > (HI_VDC_LIMIT*1086U))
  {
	PowerOff();
  }

  // Wait end of convertion
  while(AdcRegs.ADCST.bit.SEQ1_BSY == 1);      

  // Torque Control Motor A
  if(global.mca.ctrl_type >= TORQUE)
  {
    int torque;
	torque = _IQmpy(global.mva.torque, global.mca.current_max);
	moche = 1;
    utria = TorqueController(adcResults.motorA, motorA.theta_sincos, torque, &bldc_a);
  }
  else if(global.mca.ctrl_type >= FREE_LOOP)
  {
    vectors_phases torque; 
   	torque.d = 0; 
   	torque.q = global.mva.torque; 
    utria = Trans23(torque, motorA.theta_sincos);
  }

  // Torque Control Motor B
  if(global.mcb.ctrl_type >= TORQUE)
  {
   	int torque;
   	torque = _IQmpy(global.mvb.torque, global.mcb.current_max);
	moche = 0;
    utrib = TorqueController(adcResults.motorB, motorB.theta_sincos, torque, &bldc_b);
  }
  else if(global.mcb.ctrl_type >= FREE_LOOP)
  {
    vectors_phases torque; 
	  torque.d = 0; 
	  torque.q = global.mvb.torque; 
    utrib = Trans23(torque, motorB.theta_sincos);
  }

  // PWM command
  EPwm_MotorA(utria);
  EPwm_MotorB(utrib);

  // Revolution blink
  if(oldrevA != motorA.revolution ||
     oldrevB != motorB.revolution)
  {
    oldrevA = motorA.revolution;
	oldrevB = motorB.revolution;
    Leds_yellow_toggle(); 
  }

  // Sync global with eQep informations
  global.mva.index_sync_flag = motorA.index_sync_flag;
  global.mva.theta_elec = motorA.theta_elec;
  global.mva.theta_mech = motorA.theta_mech; 
  global.mva.revolution = motorA.revolution; 
  global.mvb.index_sync_flag = motorB.index_sync_flag;
  global.mvb.theta_elec = motorB.theta_elec;
  global.mvb.theta_mech = motorB.theta_mech; 
  global.mvb.revolution = motorB.revolution; 
  global.bba.busy = bangbangA.busy_flag;
  global.bbb.busy = bangbangB.busy_flag;

  // Led green toggle...
  #ifndef RELEASE
  Leds_green(1); 
  #endif                    

  // Fan Speed
  FanSpeed(global.env.fan_speed);                           

  // I2C Low SCL detect
  {
	static int low_scl_cnt = 0;
	if(GpioDataRegs.GPBDAT.bit.GPIO33 == 0)
	{
		low_scl_cnt++;
		// Low SCL detected, reset I2C module 
		if(low_scl_cnt > 200)
		{
			I2caRegs.I2CMDR.bit.IRS = 0; 
			I2caRegs.I2CMDR.bit.IRS = 1; 
		}
	}
	else
		low_scl_cnt = 0;
  }

  // Acknowledge this interrupt to receive more interrupts from group 1
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
  EPwm1Regs.ETCLR.bit.INT=1;
}
    
////////////////////////////////////////////////////////////////////////////////
/// ADC ISR                                                                     
////////////////////////////////////////////////////////////////////////////////
interrupt 
void adcIsr(void)
{
  // Save results
  AdcSaveResults(&adcResults);

  // Reinitialize for next ADC sequence
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit

  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
  AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
  AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}

////////////////////////////////////////////////////////////////////////////////
/// TorqueController                                                            
////////////////////////////////////////////////////////////////////////////////
three_phases TorqueController(three_phases currents, tri_theta angle, int torque, bldc_controller *bldc)
{
  vectors_phases ivect, uvect;
  three_phases   utri;
  
  ivect = Trans32(currents, angle);
  uvect = RegulPi_i(ivect, torque, bldc); 
  utri  = Trans23(uvect, angle);              
  utri  = Limits (utri); 

  if(moche == 1) //A
	  global.mva.power = _IQmpy(ivect.q, motorA.velocity);
  else
	  global.mvb.power = _IQmpy(ivect.q, motorB.velocity);

  return utri;
}

////////////////////////////////////////////////////////////////////////////////
/// Main Initializations                                                        
////////////////////////////////////////////////////////////////////////////////
void
InitGlobalConfiguration(void)
{
  // System version
  global.sys.version_major = 0;
  global.sys.version_minor = 7;
  global.sys.version_rev   = 1;
  global.sys.version_build = 0;
   
  // Power states
  global.env.power = POWER_OFF;
  global.env.sync = 0;
  global.env.update = 1; 

  // Init. Values
  global.mca.ctrl_type = TORQUE; 
  global.mcb.ctrl_type = TORQUE; 
  global.mva.torque = 0; 
  global.mvb.torque = 0; 
  global.mva.position = 0; 
  global.mvb.position = 0; 
  global.mva.velocity = 0; 
  global.mvb.velocity = 0; 
  global.mva.index_sync_flag = 0;
  global.mvb.index_sync_flag = 0;
  global.mca.encoder_resolution = 1024;
  global.mcb.encoder_resolution = 1024;

  // Controllers initialisations   
  global.mca.tq_kp =    500; 
  global.mca.tq_gi =   5000; 
  global.mca.sp_kp =  12000; 
  global.mca.sp_gi =   6000; 
  global.mca.pos_kp = 12500;
  global.mca.pos_gi =     0;

  global.mcb.tq_kp =    500; 
  global.mcb.tq_gi =   5000; 
  global.mcb.sp_kp =  12000; 
  global.mcb.sp_gi =   6000; 
  global.mcb.pos_kp = 12500;
  global.mcb.pos_gi =     0;

  // Maximum Values
  global.mca.current_max = 11000;  
  global.mcb.current_max = 11000; 

  global.mca.bang_acc_max = 400;
  global.mca.bang_dec_max = 400;
  global.mca.bang_spd_max = 6000;

  global.mcb.bang_acc_max = 400;
  global.mcb.bang_dec_max = 400;
  global.mcb.bang_spd_max = 6000;

  global.bba.acceleration = 0;
  global.bba.deceleration = 0;
  global.bba.velocity = 0;
  global.bba.position = 0; 
  global.bba.enable = 0;  
  global.bbb.stop = 0; 

  global.bbb.acceleration = 0;
  global.bbb.deceleration = 0;
  global.bbb.velocity = 0;
  global.bbb.position = 0; 
  global.bbb.enable = 0; 
  global.bbb.stop = 0;

  // Axis and Wheel informations
  global.mca.gear_num = 676;
  global.mca.gear_den = 49;
  global.mca.wheel_diameter = _IQ(60);
  global.mca.position_correction = _IQ(-1) - 1; 

  global.mcb.gear_num = 676;
  global.mcb.gear_den = 49;
  global.mcb.wheel_diameter = _IQ(60);   
  global.mcb.position_correction = _IQ(-1) - 1; 

  global.xyp.x = 0; 
  global.xyp.y = 0; 
}

////////////////////////////////////////////////////////////////////////////////
/// Update Configuration                                                        
////////////////////////////////////////////////////////////////////////////////
void 
UpdateConfiguration()
{
  if(global.env.update == 0) return;
  global.env.update = 0; 

  // Init BangBang parameters
  Init_bangbang(&bangbangA, 
                4000, 
                global.mca.encoder_resolution, 
                global.mca.bang_acc_max, 
                global.mca.bang_dec_max, 
                global.mca.bang_spd_max);
  Init_bangbang(&bangbangB, 
                4000, 
                global.mcb.encoder_resolution, 
                global.mcb.bang_acc_max, 
                global.mcb.bang_dec_max, 
                global.mcb.bang_spd_max); 
}  

////////////////////////////////////////////////////////////////////////////////
/// Reset Position                                                              
////////////////////////////////////////////////////////////////////////////////
void 
reset_position()
{
    pos_x = 0; 
	pos_yaw = 0; 
	global.mva.position = 0; 
	global.mvb.position = 0; 
	global.mva.revolution = 0; 
	global.mvb.revolution = 0; 

	EQep_set_zero(&motorA);
	EQep_set_zero(&motorB);
}

////////////////////////////////////////////////////////////////////////////////
/// Reset Motor A                                                               
////////////////////////////////////////////////////////////////////////////////
void 
MotorA_reset()
{
  EPwmA_PowerOff(); 
  InitControllersA(); 
  EQep_set_zero(&motorA);
  global.mva.torque = 0; 
  global.mva.velocity = 0;
  EPwmA_PowerOn(); 
}

////////////////////////////////////////////////////////////////////////////////
/// Reset Motor B                                                               
////////////////////////////////////////////////////////////////////////////////
void 
MotorB_reset()
{
  EPwmB_PowerOff(); 
  InitControllersB(); 
  EQep_set_zero(&motorB);
  global.mvb.torque = 0; 
  global.mvb.velocity = 0; 
  EPwmB_PowerOn(); 
}

////////////////////////////////////////////////////////////////////////////////
/// Power Off                                                                   
////////////////////////////////////////////////////////////////////////////////
void 
PowerOff()
{
  FanSpeed(0);           // Stop Fan
  MotorA_reset();        // Reset MotorA
  MotorB_reset();        // Reset MotorB
  EPwm_PowerOff();       // Clear ouput PWM
  bangbang_reset(&bangbangA); 
  bangbang_reset(&bangbangB);    
  Leds_red(1);           // Set Red led
  global.env.power = 0; 
  reset_position();      // Reset position values

}

////////////////////////////////////////////////////////////////////////////////
/// Power Off                                                                   
////////////////////////////////////////////////////////////////////////////////
void 
PowerOn()
{
  global.mva.sync = 0;
  global.mvb.sync = 0;
  global.env.sync = 0;  

  FanSpeed(0);          // Stop FAN
  InitControllers();    // Clear Integral Actions
  reset_position();     // Reset position values
  EPwm_PowerOn();       // Clear ouput PWM
  Leds_red(0);          // Clear Red led
  global.env.power = 1;  
}

////////////////////////////////////////////////////////////////////////////////
/// Main function.                                                              
////////////////////////////////////////////////////////////////////////////////
void 
main(void)
{
   int powered = 0; 

   // Some initializations...
   InitSysCtrl();      
   InitPieCtrl();      
   InitPieVectTable(); 
   InitLeds();         
   InitAdc();          
   InitEPwm();
               
   InitEQep();         
   InitHall();         
   InitFan();          
   InitGlobalConfiguration(); 
   InitControllers(); 
   InitI2C();

   // Remapping ISR
   REMAP_INTERRUPT(EPWM1_INT, &pwmSyncIsr);
   REMAP_INTERRUPT(ADCINT   , &adcIsr); 

   // Enable all configured interrupts
   IER |= M_INT3;                     // Set active interrupt level INT3.y
   PieCtrlRegs.PIEIER3.bit.INTx1 = 1; // Set active EPWM1_INT interrupt (INT3.x)
   IER |= M_INT1;                     // Set active interrupt level INT1.y
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1; // Set active SEQ1_INT  interrupt (INT1.x)

   // Default state: Power Off
   PowerOff(); 

   // Now we can enable interrupts
   EnableInterrupts();          

   // Leds Visualisation (just for fun)
   chenille();   
   
   // Once loop forever
   for(;;)
   {
      // Check power state
      if(powered == 0 && global.env.power == 1) 
      {
        PowerOn(); 
        powered = 1;
      }
      else if(powered == 1 && global.env.power == 0)
      {
        PowerOff(); 
        powered = 0; 
      }
      else if(global.env.power == 0)
        UpdateConfiguration(); 

	  // I2C Polling 
	  i2c_polling();
   }
} 	

////////////////////////////////////////////////////////////////////////////////
/// End of file...                                                              
////////////////////////////////////////////////////////////////////////////////
