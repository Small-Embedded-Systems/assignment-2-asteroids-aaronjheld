/* utility functions */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "model.h"

/* number functions to help game */

int randrange(int from, int to)
{
int range = to-from;
return from + rand()%range;
}


float map(float value, float lower, float upper, float min, float max) 
{
return lerp(min,max, norm(value,lower,upper));
}


float norm(float value, float min, float max)
{
return (value-min)/(max-min);
}
float lerp(float min, float max, float value)
{
return max*value+(1.0f-value)*min;
}

const float pi = 3.1415926f;
float radians(float degrees) { return pi*(degrees/180); }

