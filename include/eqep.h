#ifndef __EQEP__
#define __EQEP__ 

#include "regul.h" 
#include "hall.h" 

////////////////////////////////////////////////////////////////////////////////
/// Define the structure of the EQEP Object.                                    
////////////////////////////////////////////////////////////////////////////////
typedef struct 
{
	int theta_raw; 			// Current Position
	int theta_elec;     	// Motor Electrical angle (Q15)
    int theta_mech;     	// Motor Mechanical Angle (Q15)
    int direction;      	// Motor rotation direction (Q0)
	int index_sync_flag; 	// Index sync status

    int mech_scaler;    	// Parameter: 0.9999/total count, total count = 4000 (Q26)
    int pole_pairs;     	// Parameter: Number of pole pairs (Q0) 
    int cal_angle;     		// Parameter: Raw angular offset between encoder and phase a (Q0) 

	int theta_elec_hall; 	// Motor Electrical angle issue from a couple of three Hall sensors
	tri_theta theta_sincos; // Sine and Cosine values for Clarke transformations

	long long position; 	// Position
	long velocity;  		// Velocity
	long acceleration; 		// Acceleration
	long revolution;        // Number of revolution

	int  sine; 				// Use Encoder or Hall sensors
	int  delta_pos;  

	unsigned long oldposition;
	long long oldposition2; 
	long long latched_position;  

	volatile struct EQEP_REGS *EQepRegs; // Pointer to EQep registers
	int (*HallFunction)(void); 
} eqep;

typedef eqep *eqep_handle;                                   

////////////////////////////////////////////////////////////////////////////////
/// Default initializer for the EQEP Object.                                    
////////////////////////////////////////////////////////////////////////////////
#define POLE_PAIRS 	8 	 // 8 poles pairs for this motor
#define TOTAL_COUNT 1024 // Optical coder has 1024 counts per revolution

#define MECH_SCALER ((1/(4*TOTAL_COUNT))<<27)
#define EQEP1_DEFAULTS {0, 0, 0, 0, 0, 32767, POLE_PAIRS, 16896, 0, {0,0}, 0, 0, 0, 0, 1, 0, 0, 0, 0, &EQep1Regs, &Hall_angle2}
#define EQEP2_DEFAULTS {0, 0, 0, 0, 0, 32767, POLE_PAIRS, 16896, 0, {0,0}, 0, 0, 0, 0, 1, 0, 0, 0, 0, &EQep2Regs, &Hall_angle1}

////////////////////////////////////////////////////////////////////////////////
/// Prototypes.                                                                 
//////////////////////////////////////////////////////////////////////////////// 
void InitEQep ();
void EQep_process (eqep_handle);
void EQep_velocity (eqep_handle);
void EQep_set_zero(eqep_handle);

#endif // __EQEP__

