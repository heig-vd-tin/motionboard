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
/// @file     regul.c                                                           
/// @language ASCII/C                                                           
/// @svn      $Id: adc.c 173 2007-03-31 22:49:25Z ychevall@heig-vd.ch $         
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   
#include "adc.h"       
#include "regul.h"       
#include "IQmathLib.h"          
#include "leds.h"               

////////////////////////////////////////////////////////////////////////////////
/// Initialize ADC                                                              
////////////////////////////////////////////////////////////////////////////////
void 
InitAdc(void)
{ 
  extern void DSP28x_usDelay(Uint32 Count);  
  volatile unsigned long i, j; 

  AdcRegs.ADCREFSEL.bit.REF_SEL = 2;  // External reference, 1.500V on ADCREFIN
  AdcRegs.ADCTRL1.bit.ACQ_PS = 1;     // Acquisition window size 
  AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;   // Cascaded mode

  AdcRegs.ADCTRL3.bit.ADCBGRFDN = 3;  // Bandgap and reference circuitry is powered up
  AdcRegs.ADCTRL3.bit.ADCPWDN = 1;    // Analog circuitry inside the core is powered up
  AdcRegs.ADCTRL3.bit.ADCCLKPS = 1;   // ADCLK = HISPCLK(SYSCLK/2)/ADCCLKPS/2 = 50e6/4 = 12.5MHz

  DELAY_US(ADC_usDELAY);              // Delay before converting ADC channels 
 
  // Maximum Conversion Channels Register
  AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 10 -(1); 

  // ADC Input Channel Sequencing Control Registers
  AdcRegs.ADCCHSELSEQ1.bit.CONV00 = ADC_I1;
  AdcRegs.ADCCHSELSEQ1.bit.CONV01 = ADC_I2;
  AdcRegs.ADCCHSELSEQ1.bit.CONV02 = ADC_I3;
  AdcRegs.ADCCHSELSEQ1.bit.CONV03 = ADC_I4;
  AdcRegs.ADCCHSELSEQ2.bit.CONV04 = ADC_I5;
  AdcRegs.ADCCHSELSEQ2.bit.CONV05 = ADC_I6;
  AdcRegs.ADCCHSELSEQ2.bit.CONV06 = ADC_ALIM_REF;
  AdcRegs.ADCCHSELSEQ2.bit.CONV07 = ADC_REF15;
  AdcRegs.ADCCHSELSEQ3.bit.CONV08 = ADC_REF0;
  AdcRegs.ADCCHSELSEQ3.bit.CONV09 = ADC_I1; //ADC_TEMP;
  AdcRegs.ADCCHSELSEQ3.bit.CONV10 = 0;
  AdcRegs.ADCCHSELSEQ3.bit.CONV11 = 0;
  AdcRegs.ADCCHSELSEQ4.bit.CONV12 = 0;
  AdcRegs.ADCCHSELSEQ4.bit.CONV13 = 0;
  AdcRegs.ADCCHSELSEQ4.bit.CONV14 = 0;
  AdcRegs.ADCCHSELSEQ4.bit.CONV15 = 0;

	// Enable SOCA trigger and interrupt
  AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1; // Enable SOCA trigger
  AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ2 = 1; // Enable SOCB trigger
  AdcRegs.ADCTRL2.bit.EPWM_SOCB_SEQ = 1;  // Enable SOCB trigger

  AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;   // Enable Interrupt
  AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;   // Enable Interrupt

  // Reset ADC
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
  AdcRegs.ADCTRL2.bit.RST_SEQ2 = 1;
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
  AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;

	// Adc Calibration
	/* AdcCalibrate */
}

////////////////////////////////////////////////////////////////////////////////
/// Calibrating ADC in offset & gain                                            
////////////////////////////////////////////////////////////////////////////////
void 
AdcCalibrate(void) 
{
	const int nConvertion = 8;
	int i;
	int ref0, ref15 = 0;

	for(i=0; i<nConvertion; i++)
	{
		// Start Of Convertion

		// Integrate result
		ref0  += AdcRegs.ADCRESULT8>>4; 
		ref15 += AdcRegs.ADCRESULT7>>4;

		// Reset ADC
		AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
		AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;		 
	}

//	offset = ref0/8; 
//	gain  = ((ref15/8)-offset)/2048;
}

////////////////////////////////////////////////////////////////////////////////
/// Save Results in adc data structure                                          
////////////////////////////////////////////////////////////////////////////////
void 
AdcSaveResults(adc *p)
{
  const int ecart_i = 5000; 
  long offset; 

  p->ref0   = (int)(AdcRegs.ADCRESULT8>>4)-2048;
  p->ref15  = (int)(AdcRegs.ADCRESULT7>>4)-2048;

  p->motorA.u = -(((int)(AdcRegs.ADCRESULT0>>4)-2048+p->ref15)<<4);
  p->motorA.v = -(((int)(AdcRegs.ADCRESULT2>>4)-2048+p->ref15)<<4);
  p->motorA.w = -(((int)(AdcRegs.ADCRESULT1>>4)-2048+p->ref15)<<4);

  p->motorB.u = -(((int)(AdcRegs.ADCRESULT3>>4)-2048+p->ref15)<<4);
  p->motorB.v = -(((int)(AdcRegs.ADCRESULT5>>4)-2048+p->ref15)<<4);
  //p->motorB.w = -(((int)(AdcRegs.ADCRESULT4>>4)-2048)<<4); 
  p->motorB.w = -(p->motorB.u + p->motorB.v); // Hardware error correction

  p->dcVoltage = AdcRegs.ADCRESULT6;
  p->temperature = (AdcRegs.ADCRESULT9>>1);

  // Test de consistance des mesures
  if (p->motorA.u + p->motorA.v + p->motorA.w >  ecart_i ||
      p->motorA.u + p->motorA.v + p->motorA.w < -ecart_i )
  {
    // Problème de mesure de courant Moteur A
  }
  else
  {
    // Correction du facteur d'offset
    offset = p->motorA.u + p->motorA.v + p->motorA.w; 
    offset = _IQmpy(offset, _IQ(1.0/3.0)); 
    p->motorA.u -= offset; 
    p->motorA.v -= offset;
    p->motorA.w -= offset;
  }

  if (p->motorB.u + p->motorB.v + p->motorB.w >  ecart_i ||
      p->motorB.u + p->motorB.v + p->motorB.w < -ecart_i )
  {
    // Problème de mesure de courant Moteur B
  }
  else
  {
    // Correction du facteur d'offset
    offset = p->motorB.u + p->motorB.v + p->motorB.w;
    offset = _IQmpy(offset, _IQ(1.0/3.0));
    p->motorA.u -= offset; 
    p->motorA.v -= offset;
    p->motorA.w -= offset;
  }
}

////////////////////////////////////////////////////////////////////////////////
/// End of file.                                                                
////////////////////////////////////////////////////////////////////////////////

