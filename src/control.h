//two_phase direct_clarke_park (three_phase i, float theta);
//three_phase inverse_park_clarke (two_phase i, float theta); 

#define SQRT3 1.73205080  // sqrt(3)
#define INVSQRT3 0.5773502 // 1/sqrt(3)

#include <math.h>
#include "C28x_FPU_FastRTS.h" 

static inline void 
direct_clarke_park (float u1, float u2, float *y1, float *y2, float theta)
{
	float sinus, cosinus, isd, isq; 
 
	isd = u1; 	
	isq = (2.0*u2 + u1)*INVSQRT3; 

	sincos(theta, &sinus, &cosinus); 
	*y1 = isd*cosinus+isq*sinus;
	*y2 = -isd*sinus+isq*cosinus;

}

static inline void
inverse_park_clarke (float isd, float isq, float *ya, float *yb, float *yc, float theta)
{
	float sinus, cosinus, ia, ib; 

	sincos(theta, &sinus, &cosinus); 

	// Inverse Park
	ia = isd*cosinus - isq*sinus;
	ib = isd*sinus   + isq*cosinus;

	// Inverse Clarke
	*ya = ia;
	*yb = ( ib*SQRT3-ia)*0.5;
	*yc = (-ib*SQRT3-ia)*0.5;
}
/*
static inline three_phase
normalize (three_phase u)
{
	float min, max, offset, diff; 
	three_phase y;

	// Identify max and min vector
	if(u.u < u.v) 
	{
		max = u.v; 
		min = u.u; 
	}
	else
	{
		max = u.u; 
		min = u.v;
	}
	if (max < u.w) max = u.w;
	if (max > u.w) min = u.w; 

	// Offset Correction
	offset = (max/2.0L + min/2.0L); 
	y.u = u.u - offset; 
	y.v = u.v - offset; 
	y.w = u.w - offset; 

	// Gain Correction if
	diff = max - min; 
	if(diff > 2.0L) 
	{
		y.u = y.u/diff*2.0L;
		y.v = y.v/diff*2.0L;
		y.w = y.w/diff*2.0L; 
	}
	return y; 
}

static inline float 
pi_controller (float y, float w, float kp, float gi)
{
	float e; 
	static float iaction; 

	e = w-y; //!< Error

	iaction += gi*e; //!< Integral Action
	if(iaction > 1.0L) iaction =  1.0L; 
	if(iaction < 1.0L) iaction = -1.0L; 

	return iaction + (e * kp); //!< Output value
}



*/
