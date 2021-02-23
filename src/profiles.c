/**
 * @title   MotionBoard project v2.0
 * @file    profile.c
 * @brief   Moves profiles
 * @author  Yves Chevallier <yves.chevallier@kalios.ch>
 * @svn     $Id: profiles.c 490 2008-11-15 14:24:53Z yves $
 */
                                                                            
////////////////////////////////////////////////////////////////////////////////
       
////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "global.h"

typedef struct 
{
	// Maximum values
	float maximum_acceleration; // tick/Tech^2
	float maximum_deceleration; // tick/Tech^2
	float maximum_speed;	    // tick/Tech
	float distance; 		    // tick

	// Current values
	float acceleration; 
	float deceleration; 
	float speed; 

	// Position information
	float distance_left;

	// Internal values and flags
	float old_speed; 
	bool start_flag; 
	bool busy_flag; 
	bool direction; 
	bool stop_flag; 

} PROFILE;

////////////////////////////////////////////////////////////////////////////////
/// Initialize profile                                                          
/// @param handle Handle of this bangbang calculation                           
/// @param acceleration Maximum acceleration [1.15]    (tr/sec^2)               
/// @param deceleration Maximum deceleration [1.15]    (tr/sec^2)               
/// @param nom_speed Nominal velocity [1.15]           (tr/min)                 
/// @param final_pos Final position [64.0]             (ticks)                  
/// @param current_pos Current position [64.0]         (ticks)                  
////////////////////////////////////////////////////////////////////////////////
void init_profile(PROFILE *h, float acceleration, float deceleration, float maximum_speed, float distance, float tech)
{
	volatile int toto = (float)tech; 

	h->maximum_acceleration = acceleration; 
	h->maximum_deceleration = deceleration; 
	h->maximum_speed = maximum_speed; 
	h->distance = distance; 

	if(h->busy_flag != true)
		h->old_speed = 0.0;
	h->busy_flag = true; 
	h->start_flag = true; 
	h->stop_flag = 0; 
}

////////////////////////////////////////////////////////////////////////////////
/// Move profile processing.
/// @param handle Handle of this bangbang calculation                           
/// @param meas_position Position Measurement                                   
/// @return Next position                                                       
////////////////////////////////////////////////////////////////////////////////
float profile_process (PROFILE *h, float position)
{
 	return h->distance+position; 
}

////////////////////////////////////////////////////////////////////////////////
/// BangBang reset values                                                       
/// @param handle Handle of this bangbang calculation                           
/// @param meas_position Position Measurement                                   
/// @return Next position                                                       
////////////////////////////////////////////////////////////////////////////////
void profile_reset(PROFILE *h)
{
	h->speed = 0;
	h->busy_flag = false; 
	h->start_flag = false; 
}

////////////////////////////////////////////////////////////////////////////////
/// BangBang smooth stop
/// @param handle Handle of this bangbang calculation                           
////////////////////////////////////////////////////////////////////////////////
void profile_smooth_stop(PROFILE *h)
{
	h->stop_flag = true; 
} 


