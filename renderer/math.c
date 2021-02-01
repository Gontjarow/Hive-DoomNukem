#include "renderer.h"

double		clamp(double n, double min, double max)
{
	assert(min < max);
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	else
		return (n);
}
