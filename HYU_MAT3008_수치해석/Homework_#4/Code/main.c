#include <stdio.h>
#include <time.h>
#include "nrutil.h"
#include "nr.h"

int main()
{
    int samples[4] = {1000, 100, 10000, 100000};
    double a = -3.0, b = 4.0, m = 0.5, s = 1.5;

    for (int i = 0; i < 4; i++)
    {
        long idum = time(NULL);
        char words[51];
        int dist[21];

        printf("-------------------- uniform (ran1) --------------------\n");
        for (int j = 0; j <= 20; j++)
            dist[j] = 0;
        for (int k = 0; k < samples[i]; k++)
        {
            float x = (b - a) * ran1(&idum) + a;
            int idx = (int)(x > 0 ? 2 * x + 0.5 : 2 * x - 0.5);
            if ((idx >= -10) && (idx <= 10))
                dist[idx + 10]++;
        }
        printf("%27d samples\n", samples[i]);
        printf("%10s %10s %9s\n", "x", "p(x)", "graph:");
        for (int j = 0; j <= 20; j++)
        {
            float dd = (float)dist[j] / samples[i];
            for (int k = 1; k <= 50; k++)
                words[k] = ' ';
            int klim = (int)(200 * dd);
            if (klim > 50)
                klim = 50;
            for (int k = 1; k <= klim; k++)
                words[k] = '*';
            printf("%5.2f ~ %5.2f %8.4f  ", (float)j / 2 - 5 - 0.25, (float)j / 2 - 5 + 0.25, dd);
            for (int k = 1; k <= 50; k++)
                printf("%c", words[k]);
            printf("\n");
        }
        printf("\n");

        printf("-------------------- gaussian(gasdev) --------------------\n");
        for (int j = 0; j <= 20; j++)
            dist[j] = 0;
        for (int k = 0; k < samples[i]; k++)
        {
            float x = s * gasdev(&idum) + m;
            int idx = (int)(x > 0 ? 2 * x + 0.5 : 2 * x - 0.5);
            if ((idx >= -10) && (idx <= 10))
                dist[idx + 10]++;
        }
        printf("%27d samples\n", samples[i]);
        printf("%10s %10s %9s\n", "x", "p(x)", "graph:");
        for (int j = 0; j <= 20; j++)
        {
            float dd = (float)dist[j] / samples[i];
            for (int k = 1; k <= 50; k++)
                words[k] = ' ';
            int klim = (int)(200 * dd);
            if (klim > 50)
                klim = 50;
            for (int k = 1; k <= klim; k++)
                words[k] = '*';
            printf("%5.2f ~ %5.2f %8.4f  ", (float)j / 2 - 5 - 0.25, (float)j / 2 - 5 + 0.25, dd);
            for (int k = 1; k <= 50; k++)
                printf("%c", words[k]);
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}
