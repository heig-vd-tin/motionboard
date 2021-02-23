#pragma once

/**
 * Define the structure of the ADC Object.
 */
typedef struct {
    float i1;
    float i2;
} adc_current;

typedef struct {
    float x;
    float y;
    float z;
} adc_accelerometer;

typedef struct {
    adc_current inverter_a;
    adc_current inverter_b;

    float temperature;
    float power_voltage;
    float ref1_5;
    float ref0;
    float offset;

    adc_accelerometer accelerometer;
} adc;

typedef adc *adc_handle;

#define ADC_DEFAULTS                        \
    {                                       \
        {0, 0, 0}, {0, 0, 0}, 0, 0, 0, 0, 0 \
    }

void Init_adc(void);
void AdcCalibrate(void);
void adc_save_results(adc_handle);
