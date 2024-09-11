#include <stdio.h>
#include "eps.h"

void get_eps(float *eps)
{
	*eps = 0.f;
	float float_eps = 2.f;
	int cnt = 0;

	// epsilon 은 0보다 큰 값이기 때문에 float_eps 값이 0이 되어버린 순간 end
	while (1.f + float_eps != 1.f)
	{
		*eps = float_eps;
		float_eps /= 2;
		cnt++;
	}
	printf("Machine Accuracy (get_eps): \t%0.20f\n", *eps);
	printf("Minimum n in 1 + 2^(-n) = 1 is: %d\n", cnt);
}

void get_double_eps(double *deps)
{
	*deps = 0.0;
	double double_eps = 2.0;
	int cnt = 0;

	// epsilon 은 0보다 큰 값이기 때문에 douebl_eps 값이 0이 되어버린 순간 end
	while (1.0 + double_eps != 1.0)
	{
		*deps = double_eps;
		double_eps /= 2;
		cnt++;
	}
	printf("Machine Accuracy (get_double_eps): \t%0.20f\n", *deps);
	printf("Minimum n in 1 + 2^(-n) = 1 is: %d\n", cnt);
}

int main()
{
	int ibeta, it, irnd, ngrd, machep, negep, iexp, minexp, maxexp;
	float eps, epsneg, xmin, xmax;
	double deps, depsneg, dxmin, dxmax;
	float my_eps;
	double my_deps;

	machar(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp,
		   &eps, &epsneg, &xmin, &xmax);
	printf("Machine Accuracy (machar): \t%0.20f\n", eps);

	get_eps(&my_eps);

	machar_double(&ibeta, &it, &irnd, &ngrd, &machep, &negep, &iexp, &minexp, &maxexp,
				  &deps, &depsneg, &dxmin, &dxmax);
	printf("Machine Accuracy (machar_doble): \t%0.20f\n", deps);

	get_double_eps(&my_deps);
	return 0;
}
