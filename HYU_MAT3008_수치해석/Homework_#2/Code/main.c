#include <stdio.h>
#include "nr.h"
#include "nrutil.h"

float muller(float (*func)(float), float x1, float x2, float xacc, int *count);

void funcd(float x, float *fx, float *dfx)
{
	*fx = bessj0(x);
	*dfx = -bessj1(x);
}

float my_nonlinear0(float x)
{
	return (x - 2) * (x - 3) * (x - 5) * (x - 9);
}

float my_nonlinear1(float x)
{
	return 4 * x * x * x - 57 * x * x + 242 * x - 309;
}

void my_funcd(float x, float *fx, float *dfx)
{
	*fx = my_nonlinear0(x);
	*dfx = -my_nonlinear1(x);
}

int main()
{
	float xb1[11], xb2[11];
	int nb = 0, count, total_count;
	float xacc = (1.0e-6), root;

	zbrak(bessj0, 1.0, 10.0, 30, xb1, xb2, &nb);
	for (int i = 1; i <= nb; i++)
		printf("Bracketing of the Bessel function J0's Root %d: [%f, %f]\n", i, xb1[i], xb2[i]);

	printf("\nRoots of the Bessel function J0 using Bisection: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = rtbis(bessj0, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, Convergence speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	printf("\nRoots of the Bessel function J0 using Linear interpolation: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = rtflsp(bessj0, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, Convergence speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	printf("\nRoots of the Bessel function J0 using Secant: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = rtsec(bessj0, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, Convergence speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	printf("\nRoots of the Bessel function J0 using Newton-Raphson: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = rtnewt(funcd, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, Convergence speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	printf("\nRoots of the Bessel function J0 using Newton with bracketing: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = rtsafe(funcd, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, Convergence speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	printf("\nRoots of the Bessel function J0 using Muller: \n");
	total_count = 0;
	for (int i = 1; i <= nb; i++)
	{
		count = 0;
		root = muller(bessj0, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f, speed(Iteration): %d\n", i, root, count);
		total_count += count;
	}
	printf("Total speed(Iteration): %d\n", total_count);

	nb = 0;
	printf("\n");
	zbrak(my_nonlinear0, 1.0, 10.0, 30, xb1, xb2, &nb);
	for (int i = 1; i <= nb; i++)
		printf("Bracketing routine of my nonlinear equation's Root %d: [%f, %f]\n", i, xb1[i], xb2[i]);

	printf("\nRoots of my nonlinear equation using Newton with bracketing: \n");
	for (int i = 1; i <= nb; i++)
	{
		root = rtsafe(my_funcd, xb1[i], xb2[i], xacc, &count);
		printf("Root %d: %10.6f\n", i, root);
	}

	return 0;
}
