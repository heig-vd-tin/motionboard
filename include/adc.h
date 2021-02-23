#ifndef __ADC__
#define __ADC__

#include "regul.h" 

////////////////////////////////////////////////////////////////////////////////
/// Global constants                                                            
////////////////////////////////////////////////////////////////////////////////
#define ADC_usDELAY   5000L  // 10ms delay after powered ADC


#define ADC_ALIM_REF ADCINA0 // Power Reference
#define ADC_TEMP     ADCINB0 // Temperature sensor
#define ADC_REF15    ADCINB3 // 1.5V reference
#define ADC_REF0     ADCINA3 // 0V   reference (ADCLO)
#define ADC_I1       ADCINA5 // Current measurement of motor A
#define ADC_I2       ADCINA6
#define ADC_I3       ADCINA7

#define ADC_I4       ADCINB5 // Current measurement of motor A
#define ADC_I5       ADCINB6
#define ADC_I6       ADCINB7

////////////////////////////////////////////////////////////////////////////////
/// Define the structure of the ADC Object.                                    
////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	three_phases motorA;
	three_phases motorB;
	int temperature;
	int dcVoltage;
	int ref15;
	int ref0;
	int offset; 
} adc;

typedef adc *adc_handle;  

#define ADC_DEFAULTS {{0,0,0}, {0,0,0}, 0, 0, 0, 0, 0}

////////////////////////////////////////////////////////////////////////////////
/// Functions prototypes                                                       
////////////////////////////////////////////////////////////////////////////////
void InitAdc(void);
void AdcCalibrate(void);
void AdcSaveResults(adc_handle);

#endif

