
#include <math.h>
#define MAXIT 30

float sgn(float target)
{
	if (target < 0.0)
		return -1;
	else if (target > 0.0)
		return 1;
	else
		return 0;
}

float muller(float (*func)(float), float x1, float x2, float xacc, int *count)
{
	void nrerror(char error_text[]);
	int j;
	float p0, p1, p2, p3, fp0, fp1, fp2, a, b, c;

	p0 = x1;
	p1 = (x1 + x2) * 0.5;
	p2 = x2;

	for (j = 1; j <= MAXIT; j++)
	{
		(*count)++;
		fp0 = (*func)(p0);
		fp1 = (*func)(p1);
		fp2 = (*func)(p2);
		c = fp2;
		b = (((p0 - p2) * (p0 - p2) * (fp1 - fp2)) - ((p1 - p2) * (p1 - p2) * (fp0 - fp2))) / ((p0 - p2) * (p1 - p2) * (p0 - p1));
		a = (((p1 - p2) * (fp0 - fp2)) - ((p0 - p2) * (fp1 - fp2))) / ((p0 - p2) * (p1 - p2) * (p0 - p1));
		p3 = p2 - ((2 * c) / (b + sgn(b) * sqrtf(b * b - 4 * a * c)));
		if (fabs(p3 - p2) < xacc)
			return p3;
		p0 = p1;
		p1 = p2;
		p2 = p3;
	}
	nrerror("Maximum number of iterations exceeded in rtsec");
	return 0.0;
}
#undef MAXIT
