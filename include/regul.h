#ifndef __regul__
#define __regul__

////////////////////////////////////////////////////////////////////////////////
/// Dependances                                                                 
////////////////////////////////////////////////////////////////////////////////
#include "IQmathLib.h"

////////////////////////////////////////////////////////////////////////////////
/// Structures and types                                                        
////////////////////////////////////////////////////////////////////////////////
typedef struct
{
  _iq gi_d; 
  _iq kp_d;
  _iq gi_q;
  _iq kp_q;
  _iq iaction_d;
  _iq iaction_q;
} bldc_current;

typedef struct
{
  _iq gi;
  _iq kp;
  _iq iaction;
} bldc_velocity;

typedef struct
{
  _iq gi;
  _iq kp;
  _iq iaction;
} bldc_position;         

typedef struct
{
  bldc_current currents;
  bldc_velocity velocity;
  bldc_position position; 
} bldc_controller;     

typedef struct 
{
  int u; 
  int v; 
  int w;
} three_phases; 

typedef struct
{
  int d;
  int q;
} vectors_phases;

typedef struct 
{
  int sin; 
  int cos;
} tri_theta; 

typedef struct 
{
  _iq gi; 
  _iq kp; 
  _iq td; 

  _iq iaction; 
  _iq e_old; 
} handle_controller_pid;

////////////////////////////////////////////////////////////////////////////////
/// Global Variables                                                            
////////////////////////////////////////////////////////////////////////////////
#ifdef __regulc__
  bldc_controller bldc_a; //!< Motor A
  bldc_controller bldc_b; //!< Motor B
#else
  extern bldc_controller bldc_a; //!< Motor A
  extern bldc_controller bldc_b; //!< Motor B
#endif

////////////////////////////////////////////////////////////////////////////////
/// Function prototypes                                                         
////////////////////////////////////////////////////////////////////////////////
void InitControllers (void); 
void InitControllersA(void);
void InitControllersB(void);  
//inline vectors_phases Trans32 (three_phases i, tri_theta angle);
//three_phases   Trans23 (vectors_phases uk, tri_theta angle);
vectors_phases RegulPi_i (vectors_phases ivect, int torque, bldc_controller *bldc); 
int RegulPi_v (int y_velocity, int w_velocity, bldc_controller *bldc); 
int RegulPi_p (long y_position, long w_position, bldc_controller *bldc); 
three_phases Limits (three_phases u); 
//int RegulPid (_iq y, _iq w, handle_controller_pid * h);

#define PSAT  32767
#define NSAT -32767

////////////////////////////////////////////////////////////////////////////////
/// Direct Clarke Transformation                                                
/// @param i      courants en format 1.15                                       
/// @param angle  sin et cos en format 1.15                                     
/// @return id,iq en format 1.15                                                
////////////////////////////////////////////////////////////////////////////////
static inline vectors_phases 
Trans32 (three_phases i, tri_theta angle)
{
  #define invSQRT3 _IQ15(0.5765)
  _iq ikd, ikq, isd, isq; 
  vectors_phases ik; 

  isd = i.u;
  isq = _IQmpy(i.v, invSQRT3) - _IQmpy(i.w, invSQRT3);
  ikd = _IQmpy(isd, angle.cos)  + _IQmpy(isq, angle.sin); 
  ikq = _IQmpy(-isd, angle.sin) + _IQmpy(isq, angle.cos);
  ik.d = _IQsat(ikd, PSAT, NSAT);
  ik.q = _IQsat(ikq, PSAT, NSAT);  
  return ik;
}

////////////////////////////////////////////////////////////////////////////////
/// Inverse Clarke Transformation                                               
/// @param uk : Tension biphasées sur référentiel tournant                      
/// @param angle : Vecteur sinus et cosinus représentant l'angle mech.          
/// @return Tension triphasées u,v,w                                            
////////////////////////////////////////////////////////////////////////////////
static inline three_phases
Trans23 (vectors_phases uk, tri_theta angle)
{
  three_phases utri; 
  _iq usd, usq, tmp; 
  #define SQRT3 _IQ15(1.732)  

  usd = _IQmpy(uk.d, angle.cos) - _IQmpy(uk.q, angle.sin); 
  usq = _IQmpy(uk.d, angle.sin) + _IQmpy(uk.q, angle.cos);
  tmp = _IQmpy(usq, SQRT3); 
  utri.u = _IQsat(usd, PSAT, NSAT); 
  utri.v = _IQsat((-usd + tmp)/2, PSAT, NSAT);
  utri.w = _IQsat((-usd - tmp)/2, PSAT, NSAT);

  return utri;
}

////////////////////////////////////////////////////////////////////////////////
/// Regulateur PI générique                                                     
/// @param y : Grandeur mesurée                                                 
/// @param w : Valeur de consigne                                               
/// @param kp : Gain proportionnel                                              
/// @param gi : Gain intégral                                                   
/// @param iaction : buffer intégral (ne pas oublier d're-initialiser)          
/// @return Grandeur réglée                                                     
////////////////////////////////////////////////////////////////////////////////
static inline int 
RegulPi (_iq y, _iq w, _iq kp, _iq gi, _iq *iaction)
{
  _iq e, paction, u; 

  // Error
  e = w-y;                               

  // Integral Action
  *iaction += _IQmpy(gi, e); 
  *iaction = _IQsat((*iaction), PSAT, NSAT); 

  // Proportional Action
  paction = _IQmpy(e, kp);  

  // Result
  u = _IQsat((*iaction) + paction, PSAT, NSAT);

  return (int)u;
}


#endif
////////////////////////////////////////////////////////////////////////////////
/// End of file.                                                                
////////////////////////////////////////////////////////////////////////////////

