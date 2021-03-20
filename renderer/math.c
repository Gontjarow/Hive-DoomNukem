#include "renderer.h"

double		clamp(double n, double min, double max)
{
	assert(min <= max);
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	else
		return (n);
}

double				max(double a, double b)
{
	if (a >= b)
		return (a);
	else
		return (b);
}

double				min(double a, double b)
{
	if (a <= b)
		return (a);
	else
		return (b);
}

double				remap(double in, double old_min, double old_max, double new_min, double new_max)
{
	double old_range = old_max - old_min;
	double new_range = new_max - new_min;

	return (new_min + (((in - old_min) * new_range) / old_range));
}

double			value_changed(int initialize, double value)
{
	static double previous = 0;
	double difference;

	if (initialize != 0)
	{
		previous = value;
		return (0);
	}
	else
	{
		if (value == previous)
			return (0);
		else
		{
			printf("change %.24f %.24f\n", value, previous);
			difference = value - previous;
			previous = value;
			return (difference);
		}
	}
}
