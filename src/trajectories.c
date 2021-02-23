////////////////////////////////////////////////////////////////////////////////
///  _   _        _                            _                                
/// | | | |      |_|                          | |                               
/// | |_| |_____  _ _____           _     _ __| |                               
/// | |_  | ___ || |  _  \  _____  \ \  / // _  |                               
/// | | | | ____|| | |_| | (_____)  \ \/ /( (_| |                               
/// |_| |_|_____)|_|___  |           \__/  \____|                               
///                  __| | Haute Ecole d'Ingenieurs                             
///                 |___/  et de Gestion - Vaud                                 
///                                                                             
/// @title    Logiciel de contrôle de moteur pour la carte "motionboard"        
/// @context  Coupe suisse de robotique 2007                                    
/// @author   Y. Chevallier <nowox@kalios.ch>                                   
/// @file     trajectories.c                                                    
/// @language ASCII/C                                                           
/// @svn      $Id: trajectories.c 136 2007-03-10 18:19:49Z ychevall@heig-vd.ch $
///                                                                             
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// Includes files                                                              
////////////////////////////////////////////////////////////////////////////////
#include "device.h"     
#include "common.h"   
#include "IQmathLib.h"

#define DATA_LENGTH  	1024  // [n*Word]
#define SAMPLING_TIME	   1  // [ms]

int dpm;  // Motion Data Pointer
int dpr;  // Receive Data Pointer

int data[1]; // DATA_LENGTH

/*
void 
dataFollow()
{
  static int st;
  int wt = 0; 

  if(wt>=st)
  {
	motorA.position = data[dpm++];
  }
}*/
