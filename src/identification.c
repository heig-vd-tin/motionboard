#if 0
#include <stdlib.h>

#define SPA_LENGTH = 2048;
float* spa;
float* measures;

int spaCounter = 0;

void InitSPA()
{
	int i=0;
	spa = malloc(sizeof(char)*SPA_LENGTH);
	measures = malloc(sizeof(char)*SPA_LENGTH);
	for(i=0; i<SPA_LENGTH; i++)
	{
		spa[i] = rand()/(float)(RAND_MAX/2)-1.0;
	}
}


void SPAReset()
{
	spaCounter = 0;
}

float SPANext()
{
	if(spaCounter>=SPA_LENGTH)
	{
		spaCounter = 0;
	}
	return spa[spaCounter++];
}

void SPAStore(float data)
{
	measures[spaCounter-1] = data;
}

#endif