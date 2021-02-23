#pragma once

#include <string.h>

typedef enum enum_ctrl_type
{
    OFF,                         // Inverter is off. No pwm output
    FREE_LOOP,                   // No controllers
    TORQUE,                      // Torque controller
    VELOCITY,                    // Torque and velocity controller
    POSITION                     // Position, torque and velocity ctrl.
} Tctrl_type;

typedef enum enum_test_type
{
	DISABLED,					 // Test mode is disabled
	CURRENT_UNIT_STEP, 			 // Current logging with unit step
	VELOCITY_UNIT_STEP, 		 // Velocity logging with unit step
	POSITION_UNIT_STEP  		 // Position logging with unit step
} Ttest_type;

typedef enum enum_motion_mvt
{
    CONSTANT_SPEED,              // For very small moves
    CONSTANT_ACCELERATION,       // Optimal timing
    CUBIC_MOVE,                  // Small speed
    LINEAR_ACCELERATION,         // Small speed with minimal loss
    SINUSOID,                    // Medium speed
    CYLOID,                      // High speed
    TRAPEZOIDAL_ACCELERATION,    // Very high speed

    AUTO                         // Automatic mode
} Tmotion_mvt;

typedef enum enum_power_state
{
    POWER_OFF,
    POWER_ON
} Tpower_state;

typedef enum enum_coupling
{
    TWO_INDEPENDANTS_DRIVES,     // Two independants drives  (use MoveUnit A&B)
    MOTOR_A_AND_B_COUPLED,       // B = A*coupling_value     (use MoveUnit A)
    POLAR_COUPLING               // A = MUA+MUB, B = MUA-MUB (use MoveUnit A&B)
} Tcoupling;


typedef struct
{
    int8  version_major;            // Major release version (0,1,2,...)
    int8  version_minor;            // Minor release version (0,1,2,...)
    int8  version_rev;              // Revision value (A,B,C,...)
	int8  identification_code[16]; 	// Software product name ("Foo, Bar!")
} globalVersion;

typedef struct
{
    Tpower_state power;		// Inverter power state
    bool  sync;  		// Start an synchronous move (write 1 to sync)
    bool  update;		// Update configuration registers
    char  i2c_addr; 		// I2c address (0..127)
    char  fan_speed;		// Speed of the fan (0..100)

    char  speed_unit;           // 0=revolution per minute (regular type)
                                // 1=meter per second (linear type)
                                // 2=percent of voltage_max value

    char torque_unit;           // x=percent of current_max value

    Tcoupling coupling;         // Coupling type
    float coupling_value;       // Coupling ratio
	bool pwm_boost; 			// Gain 13% on pwm output with a non-linearity
	float pwm_frequency; 		// Usually 24e3 kHz
} globalEnvironment;

typedef struct
{
    float dc_voltage; 		// DC bus voltage
    float inverter_temperature;  // Temperature of the mosfets inverter
    bool error_flag; 		// An error has occured
    int16 error_number; 	// Last error number
} globalStatus;

typedef struct
{
  Tctrl_type ctrl_type;         // Control type (Free-loop, Torque, Pos., ...)
  Tmotion_mvt motion_mvt;       // Motion path type (Bang-bang, linear, ...)

  Uint16 encoder_resolution;    // TTL/SinCos encoder ticks for a revolution
  Uint16 poles_numbers;         // Number of magnetic poles

  // Torque controller is a PI controller
  float tq_kp;                  // Torque proportional action
  float tq_gi;                  // Torque integral action
  float current_max;            // Maximum current value [A]

  // Velocity controller is a PI controller
  float sp_kp;                  // Velocity proportional action
  float sp_gi;                  // Velocity integral action
  float sp_dz;                  // Velocity death zone
  float voltage_max;            // Maximum voltage value [V]

  // Position controller is a P controller
  float pos_kp;                 // Position proportional action
  float pos_dz;                 // Position death zone

  int16 gear_num;               // Gear numerator
  int16 gear_den;               // Gear denominator

  float hall_offset; 			// Offset (0� for 120� and 30� for 180�)

  float desat;
} globalMotorConfiguration;

typedef struct
{
  float position;   // 1 = one revolution counter clockwise direction
  float velocity;       // Motor speed (unit depend of speed_unit value)
  float torque;         // Motor torque (unit depend of torque_unit value)
  float pwm; 			// Free_loop value (+-1.0)
} globalMotorValues;

typedef struct
{
  bool  busy;           // 0 = idle state, 1 = move in progress
  bool  stop;           // write 1 to immediatly stop movement
  float acceleration;   // Acceleration value (% of max_current)
  float deceleration;   // Decelaration value (% of max_current)
  float velocity;       // Maximum velocity value (% of max_voltage)
  float position;       // Move length. Same unit has motor position value
} globalMove;

typedef struct
{
	// Configuration
	float wheel_diameter; // Size of encoder wheel in [mm]
	float axis_distance;

	// Values
	float x; 			// X position
	float y; 			// Y position
	float orientation; 	// Angle

} globalOdometry;

typedef struct
{
  volatile const globalVersion       ver; // Software version
  volatile globalEnvironment         env; // Environment version
  volatile globalStatus              sts; // System status

  volatile globalMotorConfiguration  mca; // Motor A configuration
  volatile globalMotorConfiguration  mcb; // Motor B configuration

  volatile globalMotorValues         mva; // Motor A values
  volatile globalMotorValues         mvb; // Motor B values

  volatile globalMove                bba; // Movement Unit A configuration
  volatile globalMove                bbb; // Movement Unit B configuration

  volatile globalOdometry 			 odo; // Get cartesian position

} globalConfiguration;

#define GLOBAL_DEFAULTS \
	{ \
		{2, 0, 'A', "MotionBoard v2.0"}, \
		{POWER_OFF, 0, 0, 0x2A, 0, 0, 0, TWO_INDEPENDANTS_DRIVES, 0, 0, 0.0}, \
		{0.0, 0.0, 0, 0}, \
		\
		{OFF, CONSTANT_ACCELERATION, 1024, 1, 0,0,0, 0,0,0,0,0,0,0,1, 0.0, 0.0}, \
		{OFF, CONSTANT_ACCELERATION, 1024, 1, 0,0,0, 0,0,0,0,0,0,0,1, 0.0, 0.0}, \
		\
		{0.0, 0.0, 0.0, 0.0}, \
		{0.0, 0.0, 0.0, 0.0}, \
		\
		{0,0, 0.0, 0.0, 0.0, 0.0}, \
		{0,0, 0.0, 0.0, 0.0, 0.0}, \
		\
		{0.0, 0.0, 0.0, 0.0, 0.0} \
	}

#endif

#define RECORD_LENGTH 1024
