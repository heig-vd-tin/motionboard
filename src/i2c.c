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
/// @file     i2c.c                                                             
/// @language ASCII/C                                                           
/// @svn      $Id: i2c.c 324 2008-01-21 10:51:56Z ychevall@heig-vd.ch $         
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"    
#include "common.h"  
#include "leds.h"        
#include "eqep.h"
#include "memory.h"
#include "DSP280x_I2c_defines.h" 
#include "scmb_cmd_list.h"

////////////////////////////////////////////////////////////////////////////////
/// Function prototype                                                          
////////////////////////////////////////////////////////////////////////////////
void   I2CA_Init(void);
interrupt void i2c_int1a_isr(void);

////////////////////////////////////////////////////////////////////////////////
/// Global variables                                                            
////////////////////////////////////////////////////////////////////////////////
extern volatile globalConfiguration global; 
extern eqep motorA;
extern eqep motorB;
int select_register = 0; 

#define I2C_NONE     0
#define I2C_AL       1
#define I2C_NACK     2
#define I2C_ARDY     3
#define I2C_RRDY     4
#define I2C_XRDY     5
#define I2C_STD      6
#define I2C_AAS      7 

////////////////////////////////////////////////////////////////////////////////
/// I2C Initialisation                                                          
////////////////////////////////////////////////////////////////////////////////
void 
I2CA_Init (void)
{
   I2caRegs.I2CMDR.all = 0x0000;    // Init I2C Mode Register
   I2caRegs.I2COAR = 0x70;          // Our Slave address
   I2caRegs.I2CPSC.all = 9;			// Prescaler - need 7-12 Mhz on module clk
   I2caRegs.I2CCLKL = 10;			// NOTE: must be non zero
   I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
   I2caRegs.I2CIER.bit.AAS = 1;		// Enable Interrupt "Addressed as slave"
   I2caRegs.I2CIER.bit.RRDY = 1;	// Enable Interrupt "Receive-data-ready"
   I2caRegs.I2CIER.bit.AL = 0; 
   I2caRegs.I2CIER.bit.XRDY = 1; 
   I2caRegs.I2CIER.bit.NACK = 1; 
   I2caRegs.I2CIER.bit.ARDY = 0; 

   I2caRegs.I2CIER.bit.SCD = 1; 
   I2caRegs.I2CMDR.bit.IRS = 1;     // Enable I2C
   return;   
}

////////////////////////////////////////////////////////////////////////////////
/// Read memory                                                                 
/// @param compteur de bytes (numéro du byte à envoyer)
////////////////////////////////////////////////////////////////////////////////
char 
read_memory (int reg, int byte)
{
	static long long buffer64 = 0; 
	switch(reg)
	{
	  case SCMB_REG_BB_BUSY_A: 	return global.bba.busy;
	  case SCMB_REG_BB_BUSY_B: 	return global.bbb.busy;
	  case SCMB_REG_BB_BUSY:    return (global.bbb.busy || global.bba.busy);

	  case SCMB_REG_XYP_READ_X: return (char)(global.xyp.x>>(8*byte));
	  case SCMB_REG_XYP_READ_Y: return (char)(global.xyp.y>>(8*byte));

	  case SCMB_REG_POSITION_A: 
	    if(byte == 0) buffer64 = motorA.position;
		return (char)(buffer64>>(byte*8)); 
	  case SCMB_REG_POSITION_B: 
	    if(byte == 0) buffer64 = motorB.position;
		return (char)(buffer64>>(byte*8));  
	  case SCMB_REG_LATCHED_POSITION_A: 
	    if(byte == 0) buffer64 = motorA.latched_position;
		return (char)(buffer64>>(byte*8)); 
	  case SCMB_REG_LATCHED_POSITION_B: 
	    if(byte == 0) buffer64 = motorB.latched_position;
		return (char)(buffer64>>(byte*8)); 
	  case SCMB_REG_READ_POWER_A: 
	    if(byte == 0) buffer64 = global.mva.power;
		return (char)(buffer64>>(byte*8));
	  case SCMB_REG_READ_POWER_B: 
	    if(byte == 0) buffer64 = global.mvb.power;
		return (char)(buffer64>>(byte*8));
	}	
	return 69; 
}

////////////////////////////////////////////////////////////////////////////////
/// Write memory                                                                
////////////////////////////////////////////////////////////////////////////////
void 
write_memory(char* data, int length)
{
  switch(data[0]) // Data index 0 is address pointer
  {
	case SCMB_REG_BROADCAST: // Broadcast 
		break;
	case SCMB_REG_POWER_ON: 
		if(data[1] == 0x5F) global.env.power = 1; 
		break;
	case SCMB_REG_POWER_OFF: 
		if(data[1] == 0xF5) global.env.power = 0; 
		break;
	case SCMB_REG_MODE_A:
		if(data[1] < 6)
			global.mca.ctrl_type = data[1];
		break;
	case SCMB_REG_MODE_B:
		if(data[1] < 6)
			global.mcb.ctrl_type = data[1];
		break;
	case SCMB_REG_TORQUE_A: 
		global.mva.torque = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_TORQUE_B: 
		global.mvb.torque = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_VELOCITY_A: 
		global.mva.velocity = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_VELOCITY_B: 
		global.mvb.velocity = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_ACC_A: 
		global.bba.acceleration = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_ACC_B: 
		global.bbb.acceleration = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_DEC_A: 
		global.bba.deceleration = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_DEC_B: 
		global.bbb.deceleration = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_VEL_A:
		global.bba.velocity = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_VEL_B:
		global.bbb.velocity = (int)(data[2]<<8) + (int)data[1];
		break;
	case SCMB_REG_BB_MOV_A: 
	{
		int i; 
		long long value = 0; 
		for(i = 0; i < length-1; i++)
			value += (long long)(data[i+1])<<(8*i);
		global.bba.position = value; 
		break;
	}
	case SCMB_REG_BB_MOV_B: 
	{
		int i; 
		long long value = 0; 
		for(i = 0; i < length-1; i++)
			value += (long long)(data[i+1])<<(8*i);
		global.bbb.position = value; 
		break;
	}
	case SCMB_REG_SYNC: 
		if(data[1] == 0x5F)
			global.env.sync = 1;
		break;
	case SCMB_REG_SYNC_A: 
		if(data[1] == 0x5F)
			global.mva.sync = 1;
		break;
	case SCMB_REG_SYNC_B: 
		if(data[1] == 0x5F)
			global.mvb.sync = 1;
		break;
  case SCMB_REG_ZERO_A:
      global.mva.position = 0;
    break;
  case SCMB_REG_ZERO_B:
      global.mvb.position = 0;
    break;

  // Adaptation des gains des régulateurs moteur A
  case SCMB_REG_TQ_KP_A:  global.mca.tq_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_TQ_GI_A:  global.mca.tq_gi = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_SP_KP_A:  global.mca.sp_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_SP_GI_A:  global.mca.sp_gi = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_POS_KP_A: global.mca.pos_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_POS_GI_A: global.mca.pos_gi = (int)(data[2]<<8) + (int)data[1]; break;

  // Adaptation des gains des régulateurs moteur B
  case SCMB_REG_TQ_KP_B:  global.mcb.tq_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_TQ_GI_B:  global.mcb.tq_gi = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_SP_KP_B:  global.mcb.sp_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_SP_GI_B:  global.mcb.sp_gi = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_POS_KP_B: global.mcb.pos_kp = (int)(data[2]<<8) + (int)data[1]; break;
  case SCMB_REG_POS_GI_B: global.mcb.pos_gi = (int)(data[2]<<8) + (int)data[1]; break;

  // Reset Master Position
  case SCMB_REG_RESET_POSITION_A:
	if(global.env.power == POWER_OFF)
	{
  	    EQep_set_zero(&motorA);
		global.mva.position = 0; 
	}
  	break;
  case SCMB_REG_RESET_POSITION_B:
	if(global.env.power == POWER_OFF)
	{
	    EQep_set_zero(&motorB);
		global.mva.position = 0; 
	}
	break;
  case SCMB_REG_SELECT_REGISTER: 
   select_register = (int)data[1]; 
   break; 

	case SCMB_REG_POS_CORRECTION_A: 
	{
		int i; 
		long long value = 0; 
		for(i = 0; i < length-1; i++)
			value += (long long)(data[i+1])<<(8*i);
		global.mca.position_correction = value; 
		break;
	}
	case SCMB_REG_POS_CORRECTION_B: 
	{
		int i; 
		long long value = 0; 
		for(i = 0; i < length-1; i++)
			value += (long long)(data[i+1])<<(8*i);
		global.mcb.position_correction = value; 
		break;
	}
    case SCMB_REG_LATCH_POSITIONS: 
	    motorA.latched_position = motorA.position; 
	    motorB.latched_position = motorB.position; 
		break; 
	case SCMB_REG_SMOOTH_STOP:
	    global.bba.stop = 1;
	    global.bbb.stop = 1;
		break;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// I2C Interrupt service routine                                               
/// NOTICE: Les tests effectués ont détecté un problème lors d'une lecture 
/// très soutenue. Le module se bloque. Il se peut qu'il s'agisse d'un problème
/// de stackoverflow. Aussi, le mode polling est préférable dans cette application
/// A titre d'information cette function reste présente dans ce fichier
////////////////////////////////////////////////////////////////////////////////
#define I2C_BUF_SIZE 10

volatile int rxp = 0, txp = 0, flag_xrdy = 0; 
static char rxd[I2C_BUF_SIZE];
/*
interrupt void 
i2c_int1a_isr (void)
{
	int int_src; 

	// Read 
	int_src = I2caRegs.I2CISRC.all; 

	// Le chip est adressé cela peut dire que
	// On reçois un nouveau message
	// On poursuit un message existant auquel cas
	// On dois déjà avoir reçu un XRDY
	if(int_src == I2C_AAS)
	{
		// General Call (broadcast is generated
		if(I2caRegs.I2CSTR.bit.AD0 == 1)
		{
		}
		else
		{
			// Read process is already started
			if(flag_xrdy == 1)
			{
			}
			// New data is becoming
			else
			{
				rxp = 0; // Reset rx data pointer	
				txp = 0; // Reset tx data pointer
			}
		}
	}
	// Un signal de stop est détecté
	// Dans tous les cas la transmission se termine. 
	// La donnée est écrite à condition que l'on aie reçu quelque chose
	// et qu'il ne s'agit pas d'une lecture
	else if(int_src == I2C_STD)
	{
		// Isn't read message
		if(flag_xrdy == 0 && rxp > 0)
		{
			write_memory(rxd, rxp);
		}
		flag_xrdy = 0; 
		I2caRegs.I2CSTR.bit.XRDY = 1; // Clear flags
		I2caRegs.I2CSTR.bit.ARDY = 1; 
		I2caRegs.I2CSTR.bit.RRDY = 1; 
		I2caRegs.I2CSTR.bit.NACKSNT = 1; 
		I2caRegs.I2CCNT = 0;
	}
	// On reçois un byte par conséquent
	// il faut le stocker dans le buffer
	// si le buffer est plein on emmet un NACK
	else if(int_src == I2C_RRDY)
	{
		if(rxp < I2C_BUF_SIZE)
			rxd[rxp++] = I2caRegs.I2CDRR;
		else
			I2caRegs.I2CMDR.bit.NACKMOD = 1; 
		flag_xrdy = 1; 
		// Voir la valur de 
	//	I2caRegs.I2CCNT = 0;
	}
	// Il faut préparer une donnée pour l'emission
	// L'OPCODE est contenu dans le premier byte de 
	// rxd. 
	else if(int_src == I2C_XRDY)
	{
		if(rxp == 0) rxd[0] = select_register; 
		I2caRegs.I2CDXR = read_memory(rxd[0], txp++); 
	}
	// La donnée n'a pas été transmise correctement, on reset la transmission
	else if(int_src == I2C_NACK)
	{
		flag_xrdy = 0; 
		I2caRegs.I2CSTR.bit.XRDY = 1; // Clear flags
		I2caRegs.I2CSTR.bit.ARDY = 1; 
		I2caRegs.I2CSTR.bit.RRDY = 1; 	
		I2caRegs.I2CCNT = 0;
		rxp = 0; 	
		txp = 0; 				
	}

  PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;      
}   */    

////////////////////////////////////////////////////////////////////////////////
/// I2C_Process in polling mode                                                 
////////////////////////////////////////////////////////////////////////////////
void i2c_polling()
{
	int int_src; 

	// Read 
	int_src = I2caRegs.I2CISRC.all; 

	// Le chip est adressé cela peut dire que
	// On reçois un nouveau message
	// On poursuit un message existant auquel cas
	// On dois déjà avoir reçu un XRDY
	if(int_src == I2C_AAS)
	{
		// General Call (broadcast is generated
	//	if(I2caRegs.I2CSTR.bit.AD0 == 1)
	//	{
//		}
//		else
		{
			// Read process is already started
			if(flag_xrdy == 1)
			{
			
			}
			// New data is becoming
			else
			{
				rxp = 0; // Reset rx data pointer	
				txp = 0; // Reset tx data pointer
			}
		}
	}
	// Un signal de stop est détecté
	// Dans tous les cas la transmission se termine. 
	// La donnée est écrite à condition que l'on aie reçu quelque chose
	// et qu'il ne s'agit pas d'une lecture
	else if(int_src == I2C_STD)
	{
		// Isn't read message
		if(flag_xrdy == 0 && rxp > 0)
		{
			write_memory(rxd, rxp);
		}
		flag_xrdy = 0; 
		I2caRegs.I2CSTR.bit.XRDY = 1; // Clear flags
		I2caRegs.I2CSTR.bit.ARDY = 1; 
		I2caRegs.I2CSTR.bit.RRDY = 1; 
		I2caRegs.I2CSTR.bit.NACKSNT = 1; 
		I2caRegs.I2CCNT = 0;
		rxp = 0; 
		txp = 0; 
	}
	// On reçois un byte par conséquent
	// il faut le stocker dans le buffer
	// si le buffer est plein on emmet un NACK
	else if(int_src == I2C_RRDY)
	{
		if(rxp < I2C_BUF_SIZE)
			rxd[rxp++] = I2caRegs.I2CDRR;
		else
			I2caRegs.I2CMDR.bit.NACKMOD = 1; 
		// Voir la valur de 
	//	I2caRegs.I2CCNT = 0;
	}
	// Il faut préparer une donnée pour l'emission
	// L'OPCODE est contenu dans le premier byte de 
	// rxd. 
	else if(int_src == I2C_XRDY)
	{
		if(rxp == 0) 
			rxd[0] = select_register; 
		I2caRegs.I2CDXR = read_memory(rxd[0], txp++); 
		flag_xrdy = 1; 
	}
	// La donnée n'a pas été transmise correctement, on reset la transmission
	else if(int_src == I2C_NACK)
	{
		flag_xrdy = 0; 
		I2caRegs.I2CSTR.bit.XRDY = 1; // Clear flags
		I2caRegs.I2CSTR.bit.ARDY = 1; 
		I2caRegs.I2CSTR.bit.RRDY = 1; 	
		I2caRegs.I2CCNT = 0;
		rxp = 0; 	
		txp = 0; 				
	}
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize i2c GPIO.                                                        
////////////////////////////////////////////////////////////////////////////////
void InitI2CGpio()
{
    EALLOW;
	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 3;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 3;  // Asynch input GPIO33 (SCLA)
	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 1;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 1;   // Configure GPIO33 for SCLA operation	
    EDIS;
}    

////////////////////////////////////////////////////////////////////////////////
/// Reset I2C module.                                                           
////////////////////////////////////////////////////////////////////////////////
void
I2C_reset (void)
{
	I2caRegs.I2CMDR.bit.IRS  = 0; // Disable I2C module
	I2caRegs.I2CMDR.bit.IRS  = 1; // Enable I2C module
}

////////////////////////////////////////////////////////////////////////////////
/// Initialize i2c.                                                             
/// Cette fonction initialise le bus I2C sur le TMS2806, elle doit être appelée 
/// Lors de l'initialisation du DSP                                             
////////////////////////////////////////////////////////////////////////////////
void 
InitI2C (void)
{
 //  EALLOW;	// This is needed to write to EALLOW protected registers
 //  PieVectTable.I2CINT1A = &i2c_int1a_isr;
 //  EDIS;   // This is needed to disable write to EALLOW protected registers 
   I2CA_Init();

   InitI2CGpio(); 
 //  PieCtrlRegs.PIEIER8.bit.INTx1 = 1;
 //  IER |= M_INT8;
}

