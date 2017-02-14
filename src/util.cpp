
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>


double
get_time(void)
{
	struct timeval tv;
	double t;

	if (gettimeofday(&tv, NULL) < 0)
		printf("carmen_get_time encountered error in gettimeofday : %s\n", strerror(errno));

	t = tv.tv_sec + tv.tv_usec/1000000.0;
	return t;
}


double
unary_rand()
{
	return (double) rand() / (double) RAND_MAX;
}


double
my_normal_distribution(double std)
{
	double normal_sample = 0;

	for (int i = 0; i < 12; i++)
		normal_sample += std * (2.0 * (unary_rand() - 0.5));

	return normal_sample / 2.0;
}


