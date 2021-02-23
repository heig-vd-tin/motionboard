
////////////////////////////////////////////////////////////////////////////////
/// Global definitions                                                          
////////////////////////////////////////////////////////////////////////////////
#define OFF          0
#define FREE_LOOP    1
#define TORQUE       2
#define VELOCITY     3
#define POSITION     4
#define BANGBANG     5
#define MOTION_PATH  6

#define POWER_OFF    0
#define POWER_ON     1

#define TWOAXIS      0
#define POLAR        1

////////////////////////////////////////////////////////////////////////////////
/// System Information                                                          
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  Uint8  version_major; 
  Uint8  version_minor; 
  Uint8  version_rev;
  Uint8  version_build; 
  Uint8  version_string[16];    
} globalSystem;

////////////////////////////////////////////////////////////////////////////////
/// Environment Information                                                     
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  Uint8  power;
  Uint8  sync; 
  Uint8  update;
  Uint8  dc_voltage;	  // DC Power Voltage 1LSB = 0.2V
  Uint8  coupling; 
  Uint8  warning_flg;			
  Uint8  error_flag;				
  Uint8  i2c_addr; 
  Uint8  fan_speed;
  Uint8  temp_value; 
  Uint8  overload_flag;  
} globalEnvironment;

////////////////////////////////////////////////////////////////////////////////
/// Motor Configuration                                                         
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  Uint8  ctrl_type;

  Uint16 encoder_resolution; 

  Uint16 tq_kp;
  Uint16 tq_gi; 
  Uint16 current_max;

  Uint16 sp_kp; 
  Uint16 sp_gi;
  Uint16 velocity_max;

  Uint16 pos_kp; 
  Uint16 pos_gi; 
  Uint8  pos_dz; 
  Uint32 pos_min; 
  Uint32 pos_max;

  int16 bang_acc_max; 
  int16 bang_dec_max; 
  int32 bang_spd_max; 

  int32 gear_num;
  int32 gear_den;
  int32 wheel_diameter; 

  int32 position_correction;

} globalMotorConfiguration;

////////////////////////////////////////////////////////////////////////////////
/// Motors Values                                                               
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  long long position; 
  int8  sync; 
  int16 velocity; 
  int16 torque; 
  int16 theta_elec; 
  int16 theta_mech; 
  int16 index_sync_flag;   
  int16 revolution; 
  int16 power; 

} globalMotorValues;

////////////////////////////////////////////////////////////////////////////////
/// Motion Path                                                                 
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  Uint8  start;
  Uint8  stop;
  Uint8  time_interval;
  Uint16 delta_position[100];  
} globalMotionPath;

////////////////////////////////////////////////////////////////////////////////
/// Bang Bang                                                                   
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  int8  enable;
  int8  busy;
  int8  stop; 
  int16 acceleration;
  int16 deceleration;
  int16 velocity;
  int32 position; 
  int32 position_mm;
} globalBangBang;

////////////////////////////////////////////////////////////////////////////////
/// X/Y Positionning                                                            
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
   int8  enable;
   int32 entrax;
   int64 x;
   int64 y;  
} xyPositionning;

////////////////////////////////////////////////////////////////////////////////
/// Global Structure                                                            
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  volatile globalSystem              sys; 
  volatile globalEnvironment         env; 
  volatile globalMotorConfiguration  mca;
  volatile globalMotorConfiguration  mcb;  

  volatile globalMotorValues         mva;
  volatile globalMotorValues         mvb; 

  volatile globalMotionPath          mpa; 
  volatile globalMotionPath          mpb; 
  
  volatile globalBangBang            bba;
  volatile globalBangBang            bbb;

  volatile xyPositionning            xyp;
} globalConfiguration;

////////////////////////////////////////////////////////////////////////////////
/// End of file                                                                 
////////////////////////////////////////////////////////////////////////////////

