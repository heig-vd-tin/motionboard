/**
 * Interrupt service routines module
 */
#include "accelerometer.h"
#include "adc.h"
#include "control.h"
#include "eqep.h"
#include "global.h"
#include "hall.h"
#include "homing.h"
#include "leds.h"
#include "memory.h"
#include "pwm.h"

extern globalConfiguration conf;
// extern float spa[8192];

float integral1 = 0.0, integral2 = 0.0;
float integral3 = 0.0, integral4 = 0.0;
float integral5 = 0.0, integral6 = 0.0;

adc adc_results;
eqep eqep1 = EQEP1_DEFAULTS;
eqep eqep2 = EQEP2_DEFAULTS;

two_phase w1 = {0, 0}, w2 = {0, 0};

float window;

typedef enum { HALL, TTL, SINCOS, RESOLVER } Telectrical_angle_src;

Telectrical_angle_src electrical_angle_src = HALL;

float e;

int current_log = 0;
int identification = 0;

int iplot = 0;
// float yplot[8192];

float amp = 0.1;

void reset_current_integral()
{
    integral1 = 0.0;
    integral2 = 0.0;
    integral3 = 0.0;
    integral4 = 0.0;
}

float pi_controller(float w, float y, float kp, float gi, float *ui)
{
    float e, u;
    e = w - y;

    (*ui) += e;

    u = kp * (e + (*ui) * gi);

    if (u > 1.0) {
        u = 1.0;
        (*ui) -= e;
    }
    if (u < -1.0) {
        u = -1.0;
        (*ui) -= e;
    }

    return u;
}

float pi_controller_dz(float w, float y, float kp, float gi, float *integral,
                       float window)
{
    float e;
    e = w - y;

    if (e < window && e > -window) e = 0;

    (*integral) += e * gi;
    if (*integral > 1.0) (*integral) = 1.0;
    if (*integral < -1.0) (*integral) = -1.0;

    return kp * e + *integral;
}

float pid_controller_dz(float w, float y, float kp, float gi, float td,
                        float *ui, float window, float *e_old)
{
    float e, u, ud;
    e = w - y;

    if (e < window && e > -window) e = 0;

    (*ui) += e;
    ud = e - (*e_old);
    (*e_old) = e;

    u = kp * (e + (*ui) * gi + td * ud);

    if (u > 1.0) {
        u = 1.0;
        (*ui) -= e;
    }
    if (u < -1.0) {
        u = -1.0;
        (*ui) -= e;
    }

    return u;
}

float p_controller_dz(float w, float y, float kp, float window)
{
    float e;
    e = w - y;

    if (e < window && e > -window) e = 0;

    return kp * e;
}

two_phase u1, u2, y1, y2;
three_phase pwm1, pwm2;
float theta1 = 0.0, theta2 = 0.0;

#define H1a (unsigned int)GpioDataRegs.GPBDAT.bit.GPIO62
#define H2a (unsigned int)GpioDataRegs.GPBDAT.bit.GPIO61
#define H3a (unsigned int)GpioDataRegs.GPBDAT.bit.GPIO63

float mypos = 0.0;

int HallPosition()
{
    if (H1a) {
        if (H2a) {
            return 4;
        } else {
            if (H3a) {
                return 2;
            } else {
                return 3;
            }
        }
    } else {
        if (H2a) {
            if (H3a) {
                return 0;
            } else {
                return 5;
            }
        } else {
            return 1;
        }
    }
}

int countStep = 0;
int countRevolution = 0;
int hallPosition = 0, hallOldPosition = 0;
float velocity = 0.0;
float nano = 0.0;
float HallGetSpeed()
{
    float dspeed = 0.0;

    hallPosition = HallPosition();

    countStep++;
    countRevolution++;

    if (countStep > 2000) {
        velocity = 0;
        countStep = 1999;
        countRevolution = 1999;
    }
    if (hallOldPosition != hallPosition) {
        // Compute Speed
        if (countStep < 100) {
            // Number of count 0 (High Speed)
            if (hallPosition == 0)
                velocity = (60 * conf.env.pwm_frequency) /
                           (conf.mca.poles_numbers * countRevolution);  // rpm
        } else {
            // Number of count 0 - 1 (Low and Medium Speed)
            velocity = (60 * conf.env.pwm_frequency) /
                       (6.0 * conf.mca.poles_numbers * countStep);  // rpm
        }
        if (hallPosition == 0) countRevolution = 0;
        countStep = 0;

        // Bla
        if (hallOldPosition == 5 && hallPosition == 0) {
            dspeed = -1;
            nano = -velocity;
        } else if (hallOldPosition == 0 && hallPosition == 5) {
            dspeed = +1;
            nano = +velocity;
        } else {
            dspeed = -(hallPosition - hallOldPosition);
            nano = dspeed * velocity;
        }
        hallOldPosition = hallPosition;
    }
    mypos += dspeed;

    return velocity;
}
float old_theta1 = 0;
float toto = 0, titi = 0;
int sens = 0;
int meilleur = 0;

interrupt void pwm_sync_isr(void)
{
    // Get electrical angle for current controllers
    /*	switch(electrical_angle_src)
            {
            case HALL:*/
    theta1 = Hall_angle1();
    theta2 = Hall_angle2();
#if 0
	if(meilleur == 1)
	{
		toto++;
		if(theta1 != old_theta1)
		{
			if(theta1 > old_theta1)
				sens = 1;
			else
				sens = 0;
			titi = toto;
			toto = 0;
			old_theta1 = theta1;
		}

	}
	if(titi > 5)
	{
		if(sens == 0)
			theta1 += (1.047/titi)*toto;
		else
			theta1 -= (1.047/titi)*toto;
	}
#endif
    //	HallGetSpeed();
    //	theta2 = Hall_angle2 ();
    /*		break;
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
            }*/
    // theta1 += 0.001;

    // ADC get results
    while (AdcRegs.ADCST.bit.SEQ1_BSY == 1)
        ;
    adc_save_results(&adc_results);
    conf.sts.dc_voltage = adc_results.power_voltage;
    conf.sts.inverter_temperature = adc_results.temperature;

    // Direct Park and Clarke
    direct_clarke_park(adc_results.inverter_a.i1, adc_results.inverter_a.i2,
                       &y1.d, &y1.q, theta1);
    direct_clarke_park(adc_results.inverter_b.i1, adc_results.inverter_b.i2,
                       &y2.d, &y2.q, theta2);

    // Currents controllers
    if (conf.mca.ctrl_type >= TORQUE) {
        u1.d = pi_controller(conf.mca.desat, y1.d, conf.mca.tq_kp,
                             conf.mca.tq_gi, &integral2);
        conf.mva.pwm = pi_controller(conf.mva.torque, y1.q, conf.mca.tq_kp,
                                     conf.mca.tq_gi, &integral1);
    } else
        u1.d = 0.0;

    if (conf.mcb.ctrl_type >= TORQUE) {
        u2.d = pi_controller(conf.mcb.desat, y2.d, conf.mcb.tq_kp,
                             conf.mcb.tq_gi, &integral4);
        conf.mvb.pwm = pi_controller(conf.mvb.torque, y2.q, conf.mcb.tq_kp,
                                     conf.mcb.tq_gi, &integral3);
    } else
        u2.d = 0.0;

// Actuator identification
#if 0
	if(identification == 1)
	{
		u1.d = 0;
		conf.mva.pwm = spa[iplot];
		yplot[iplot] = conf.mva.pwm;
		if(++iplot >= 8192)
		{
		 	identification = 0;
			conf.mva.torque = 0;
			iplot = 0;
		}
	}
#endif

    // Inverse Park and Clarke
    if (conf.mca.ctrl_type >= FREE_LOOP) {
        inverse_park_clarke(u1.d, conf.mva.pwm, &pwm1.u, &pwm1.v, &pwm1.w,
                            theta1);
        epwm_motor_a(pwm1.u, pwm1.v, pwm1.w);
    } else
        epwm_motor_a(0, 0, 0);

    if (conf.mcb.ctrl_type >= FREE_LOOP) {
        inverse_park_clarke(u2.d, conf.mvb.pwm, &pwm2.u, &pwm2.v, &pwm2.w,
                            theta2);
        epwm_motor_b(pwm2.u, pwm2.v, pwm2.w);
    } else
        epwm_motor_b(0, 0, 0);
#if 0
	if(current_log == 1)
	{
		yplot[iplot] = y1.q;
		if(++iplot >= 8192)
		{
			current_log = 0;
			conf.mva.torque = 0;
			iplot = 0;
		}
		else
		{
			current_log = 1;
			conf.mva.torque = 0.5;
		}
	}
#endif

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
    EPwm1Regs.ETCLR.bit.INT = 1;
}

interrupt void adc_isr(void)
{
#if 0
	if(EPwm1Regs.ETFLG.bit.SOCA)
	{
		// Do nothing the first 6 conversions have done
	}
	else
#endif
    {
        AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;
    }

    EPwm1Regs.ETCLR.bit.SOCA = 1;
    EPwm2Regs.ETCLR.bit.SOCB = 1;

    // Leds_yellow_toggle();

    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;  // Acknowledge interrupt to PIE
}

interrupt void cpu_timer0_isr() { PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; }

float l1;
float l2;

float ud1 = 0;
float ud2 = 0;

float position1_save = 0;
float position2_save = 0;
int perte = 0;
int unefois = 0;

interrupt void cpu_timer1_isr()
{
    // Position and Velocity processing from TTL coders
    EQep_process(&eqep1);
    EQep_process(&eqep2);

    l1 = eqep1.position;
    l2 = eqep2.position;

    // Contact retrouv�
    if (perte == 0) {
        if (unefois == 1) {
            eqep1.position = 0;  // position1_save;
            eqep2.position = 0;  // position2_save;

            conf.mca.ctrl_type = POSITION;
            conf.mcb.ctrl_type = POSITION;

            unefois = 0;
        }
    }

    // Si perte de contact avec le sol
    if (HOME_A || BW_B) {
        conf.mva.torque = 0;
        conf.mvb.torque = 0;

        position1_save = eqep1.position;
        position2_save = eqep2.position;
        perte = 1;
        unefois = 1;
        conf.mca.ctrl_type = OFF;
        conf.mcb.ctrl_type = OFF;
    } else {
        perte = 0;
        // Régulation de position
        if (conf.mcb.ctrl_type >= POSITION) {
            conf.mvb.torque = pid_controller_dz(
                conf.mva.position, l1, conf.mcb.sp_kp, conf.mcb.sp_gi,
                conf.mcb.sp_dz, &integral5, conf.mcb.pos_dz, &ud1);
        }
        if (conf.mca.ctrl_type >= POSITION) {
            conf.mva.torque = pid_controller_dz(
                conf.mvb.position, l2, conf.mca.sp_kp, conf.mca.sp_gi,
                conf.mca.sp_dz, &integral6, conf.mca.pos_dz, &ud2);
        }
    }
#if 0
	// Position controllers
	if(conf.mca.ctrl_type >= POSITION)
	{
		conf.mva.velocity = p_controller_dz(conf.mva.position, mypos, conf.mca.pos_kp, conf.mca.pos_dz);
	}
	if(conf.mcb.ctrl_type >= POSITION)
	{
		conf.mvb.velocity = p_controller_dz(conf.mvb.position, mypos, conf.mcb.pos_kp, conf.mcb.pos_dz);
	}

	// Velocity controllers
	if(conf.mca.ctrl_type >= VELOCITY)
	{
		conf.mva.torque = pi_controller_dz(conf.mva.velocity, nano*0.0002, conf.mca.sp_kp, conf.mca.sp_gi, &integral5, conf.mca.sp_dz);
	}
	if(conf.mcb.ctrl_type >= VELOCITY)
	{
		conf.mvb.torque = pi_controller_dz(conf.mvb.velocity, nano*0.0002, conf.mcb.sp_kp, conf.mcb.sp_gi, &integral6, conf.mcb.sp_dz);
	}
#endif

    // Odometry
    EQep_xy(&eqep1, &eqep2, (float *)&conf.odo.x, (float *)&conf.odo.y,
            (float *)&conf.odo.orientation);
}

interrupt void i2c_int1a_isr(void) {}
