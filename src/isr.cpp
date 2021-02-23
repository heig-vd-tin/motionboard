#include "global.h"
#include "adc.h"
#include "leds.h"
#include "pwm.h"
#include "hall.h"
#include "adc.h"
#include "eqep.h"
#include "control.h"
#include "accelerometer.h"
#include "memory.h"

extern globalConfiguration conf;
extern class C_PiController;

adc  adc_results;

eqep  eqep1 = EQEP1_DEFAULTS;
eqep  eqep2 = EQEP2_DEFAULTS;

two_phase w1 = {0,0}, w2 = {0,0};

typedef enum {
	HALL,
	TTL,
	SINCOS,
	RESOLVER
} Telectrical_angle_src;

Telectrical_angle_src electrical_angle_src = HALL;

float e;
static float integral = 0.0;

float wplot[250];
float yplot[250];
float amp = 0.1;

float pi_controller (float w, float y, float kp, float gi)
{
	e = w-y;

	integral += e*gi;
	if(integral > 1.0) integral = 1.0;
	if(integral < -1.0) integral = -1.0;

	return kp*e+integral;
}

two_phase u1, u2, y1, y2;
three_phase pwm1, pwm2;

interrupt void pwm_sync_isr(void)
{
	float theta1=0.0, theta2=0.0;

	static int i = 0;
	static int j = 0;

	theta1 ++;
	// Get electrical angle for current controllers
	#if 0
	switch(electrical_angle_src)
	{
	case HALL:
		theta1 = Hall_angle1 ();
		theta2 = Hall_angle2 ();
		break;
	case TTL:
		if(eqep1.index_sync_flag)
			theta1 = EQep_electrical_angle(&eqep1);
		else
			theta1 = Hall_angle1();
		if(eqep2.index_sync_flag)
			theta2 = EQep_electrical_angle(&eqep2);
		else
			theta2 = Hall_angle2();
		break;
	case SINCOS:
		break;
	case RESOLVER:
		break;
	}
	theta1 = 0;
	theta2 = 0;
	#endif

	// ADC get results
	while(AdcRegs.ADCST.bit.SEQ1_BSY == 1);
	adc_save_results (&adc_results);
	conf.sts.dc_voltage = adc_results.power_voltage;
	conf.sts.inverter_temperature = adc_results.temperature;

	// Direct Park and Clarke
	direct_clarke_park(adc_results.inverter_a.i1, adc_results.inverter_a.i2, &y1.d, &y1.q, theta1);
	direct_clarke_park(adc_results.inverter_b.i1, adc_results.inverter_b.i2, &y2.d, &y2.q, theta2);

	#if 0
	if(i++>10)
	{
		i=0;
		if(j++>=250) j=0;
		if(j>125) w1.d = amp;
		else w1.d = -amp;

		wplot[j] = w1.d;
		yplot[j] = y1.d;
	}
	#endif

	// Currents controllers
	u1.d = pi_controller(w1.d, y1.d, conf.mca.tq_kp, conf.mca.tq_gi);
	u1.q = pi_controller(0.0 , y1.q, conf.mca.tq_kp, conf.mca.tq_gi);

	u2.d = pi_controller(w2.d, y1.d, conf.mcb.tq_kp, conf.mcb.tq_gi);
	u2.q = pi_controller(0.0 , y2.q, conf.mcb.tq_kp, conf.mcb.tq_gi);

	// Inverse Park and Clarke
	inverse_park_clarke(u1.d, u1.q, &pwm1.u, &pwm1.v, &pwm1.w, theta1);
	inverse_park_clarke(u2.d, u2.q, &pwm2.u, &pwm2.v, &pwm2.w, theta2);

	// Update ePWM values
	epwm_motor_a(pwm1.u,pwm1.v,pwm1.w);
	epwm_motor_b(pwm2.u,pwm2.v,pwm2.w);

	// Acknowledge this interrupt to receive more interrupts from group 1
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
	EPwm1Regs.ETCLR.bit.INT=1;
}

interrupt
void adc_isr (void)
{
	if(EPwm1Regs.ETFLG.bit.SOCA)
	{
		// Do nothing the first 6 conversions have done
	}
	else
	{
		AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
	}

 	EPwm1Regs.ETCLR.bit.SOCA = 1;
 	EPwm2Regs.ETCLR.bit.SOCB = 1;

 	// Leds_yellow_toggle();

	AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}

interrupt void
cpu_timer0_isr()
{
	Leds_red_toggle();

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

float x,y, cap;

interrupt void
cpu_timer1_isr()
{
	#if 0
  	Leds_green(0);
	EQep_process(&eqep1);
	EQep_process(&eqep2);
  	Leds_green(1);
	Leds_green(0);
	EQep_xy(&eqep1, &eqep2, &x, &y, &cap);

	Leds_green(1);

	// Velocity controllers
	//	conf.mva.torque = pi_controller(qep_a.speed, conf.mva.velocity, conf.mca.sp_kp, conf.mca.sp_gi);
	//	conf.mvb.torque = pi_controller(qep_b.speed, conf.mvb.velocity, conf.mcb.sp_kp, conf.mcb.sp_gi);
	#endif

	Leds_green_toggle();
}

