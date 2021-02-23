#include "device.h"
#include "common.h"
#include "IQmathLib.h"

#define DATA_LENGTH  	1024  // [n*Word]
#define SAMPLING_TIME	   1  // [ms]

int dpm;  // Motion Data Pointer
int dpr;  // Receive Data Pointer

int data[1]; // DATA_LENGTH


