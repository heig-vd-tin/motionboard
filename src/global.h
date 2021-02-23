
// TI File $Revision: /main/1 $
// Checkin $Date: April 22, 2008   14:35:56 $
//###########################################################################
//
// FILE:   DSP28x_Project.h
//
// TITLE:  DSP28x Project Headerfile and Examples Include File
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP28x_PROJECT_H
#define DSP28x_PROJECT_H

#include "dsp2833x/DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "dsp2833x/DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "iq_math/IQmathLib.h" 

#include "gpio.h"
#include "memory.h"

#define true 1 
#define false 0 

#define PI 3.1415926535 

typedef struct 
{
  int sin; 
  int cos;
} tri_theta; 
          

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
	float u;
	float v;
	float w;
} three_phase;

typedef struct
{
	float d;
	float q;
} two_phase;

#define REMAP_INTERRUPT(A,B) EALLOW; PieVectTable.A = B; EDIS
#define ENABLE_INT_LVL(A) IER |= A
#define DISABLE_INT_LVL(A) IER &= ~A

#endif

