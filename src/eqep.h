#pragma once

#include "global.h"

// Define the structure of the EQEP Object.
typedef struct
{
	int poles_pairs; 		// Number of pole pairs
	float total_count; 		// Optical coder resolution
	float cal_angle; 		// Raw angular offset between encoder and phase A

	bool index_sync_flag; 	// Received index pulse
	bool direction;			// Motor direction
	float electrical_angle; // Electrical angle in radian

	float position; 		// Motor position
	float velocity; 		// Motor velocity

	long old_position; 		// For internal usage
	float dposition;        // For internal usage

	volatile struct EQEP_REGS *EQepRegs; // Pointer to EQep registers
} eqep;

typedef eqep *eqep_handle;

// Default initializer for the EQEP Object.
#define M1_POLE_PAIRS  8 	 	// 8 poles pairs for this motor
#define M1_TOTAL_COUNT 4096 	// Optical coder has 1024 counts per revolution
#define M1_CAL_ANGLE   0.0      // Raw angular offset between encoder and phase A

#define M2_POLE_PAIRS  8 	 	// 8 poles pairs for this motor
#define M2_TOTAL_COUNT 4096 	// Optical coder has 1024 counts per revolution
#define M2_CAL_ANGLE   0.0      // Raw angular offset between encoder and phase A

#define EQEP1_DEFAULTS {M1_POLE_PAIRS, M1_TOTAL_COUNT, M1_CAL_ANGLE, 0, 0, 0.0, 0.0, 0.0, 0, 0.0, &EQep1Regs}
#define EQEP2_DEFAULTS {M2_POLE_PAIRS, M2_TOTAL_COUNT, M2_CAL_ANGLE, 0, 0, 0.0, 0.0, 0.0, 0, 0.0, &EQep2Regs}


void InitEQep ();
void EQep_process (eqep_handle);
void EQep_velocity (eqep_handle);
void EQep_set_zero(eqep_handle);
void EQep_xy (eqep_handle left, eqep_handle right, float *x, float *y, float *cap);
