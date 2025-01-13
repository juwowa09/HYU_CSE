#include <stdio.h>
#include <stdlib.h>
#include "nrutil.h"
#include "nr.h"
#define N 20

int main()
{
    FILE *fp;
    int *indx;
    int n, m, row, col;
    float **A, **b, **x, **Ai, **lu, **l, **u, **U, **V;
    float arr_b[N], arr_x[N], arr_i[N], arr_w[N], det;

    char *line_arr[3] = {"lineq1.dat", "lineq2.dat", "lineq3.dat"};

    A = matrix(1, N, 1, N);
    b = matrix(1, N, 1, N);
    x = matrix(1, N, 1, N);
    Ai = matrix(1, N, 1, N);

    lu = matrix(1, N, 1, N);
    l = matrix(1, N, 1, N);
    u = matrix(1, N, 1, N);
    indx = ivector(1, N);

    U = matrix(1, N, 1, N);
    V = matrix(1, N, 1, N);

    for (int i = 0; i < 3; i++)
    {
        if (!(fp = fopen(line_arr[i], "r")))
        {
            printf("Not Open file.\n");
            return 1;
        }
        fscanf(fp, "%d %d", &n, &m);

        printf("\n----------------- Equation %d -----------------\n", i + 1);
        printf("-------------------- A / b -------------------\n");
        for (int row = 1; row <= n; row++)
        {
            for (int col = 1; col <= n; col++)
                fscanf(fp, "%f", &A[row][col]);
        }
        for (row = 1; row <= n; row++)
            fscanf(fp, "%f ", &b[row][1]);

        for (int row = 1; row <= n; row++)
        {
            for (int col = 1; col <= n; col++)
            {
                printf("%15.6f", A[row][col]);
            }
            printf("     /   %15.6f", b[row][1]);
            printf("\n");
        }
        fclose(fp); // 파일 닫기

        // Gauss-Jordan 소거법으로 문제 해결
        printf("\n---------- 1) Gauss-Jordan Elimination ----------\n");
        for (row = 1; row <= n; row++)
        {
            for (col = 1; col <= n; col++)
                Ai[row][col] = A[row][col];
            x[row][1] = b[row][1];
        }

        if (!gaussj(Ai, n, x, m))
        {
            printf("\nSolution (x):\n");
            for (int row = 1; row <= n; row++)
            {
                printf("x[%d] = %15.6f\n", row, x[row][1]);
            }

            // 역행렬을 출력 (A는 역행렬로 변환됨)
            printf("\nInverse of matrix A:\n");
            for (int row = 1; row <= n; row++)
            {
                for (int col = 1; col <= n; col++)
                {
                    printf("%15.6f ", Ai[row][col]);
                }
                printf("\n");
            }
        }
        else
        {
            printf("\nThis A Matrix is singular\n");
        }

        printf("\n---------- 2) LU Decomposition ----------\n");
        for (row = 1; row <= n; row++)
        {
            for (col = 1; col <= n; col++)
                lu[row][col] = A[row][col];
            x[row][1] = b[row][1];
        }
        for (row = 1; row <= n; row++)
        {
            arr_x[row] = x[row][1];
            arr_b[row] = b[row][1];
        }
        if (!ludcmp(lu, n, indx, &det))
        {
            lubksb(lu, n, indx, arr_x);
            printf("\nSolution x by using LU Decomposition: \n");
            for (row = 1; row <= n; row++)
                printf("%15.6f\n", arr_x[row]);

            printf("\nImproved Solution x by using LU Decomposition: \n");
            mprove(A, lu, n, indx, arr_b, arr_x);
            for (row = 1; row <= n; row++)
                printf("%15.6f\n", arr_x[row]);

            printf("\nInverse of matrix A by using LU Decomposition: \n");
            for (row = 1; row <= n; row++)
                for (col = 1; col <= n; col++)
                    Ai[row][col] = 0.0;
            for (col = 1; col <= n; col++)
            {
                for (row = 1; row <= n; row++)
                    arr_i[row] = 0.0;
                arr_i[col] = 1.0;
                lubksb(lu, n, indx, arr_i);
                for (row = 1; row <= n; row++)
                    Ai[row][col] = arr_i[row];
            }
            for (row = 1; row <= n; row++)
            {
                for (col = 1; col <= n; col++)
                    printf("%15.6f", Ai[row][col]);
                printf("\n");
            }
        }
        else
        {
            det = 0;
            printf("\nMatrix A is Singular matrix\n");
        }
        printf("\n---------- 3) Singular Value Decomposition ----------\n");
        for (row = 1; row <= n; row++)
        {
            for (col = 1; col <= n; col++)
            {
                U[row][col] = A[row][col];
                V[row][col] = 0.0;
            }
            x[row][1] = b[row][1];
            arr_w[row] = 0.0;
        }
        svdcmp(U, m, n, arr_w, V);
        for (row = 1; row <= n; row++)
            for (col = 1; col <= n; col++)
                Ai[row][col] = 0.0;
        for (row = 1; row <= m; row++)
        {
            for (col = 1; col <= n; col++)
            {
                Ai[row][col] = 0.0;
                for (int j = 1; j <= n; j++)
                {
                    if (arr_w[j] == 0)
                        Ai[row][col] += V[row][j] * 0 * U[col][j];
                    else
                        Ai[row][col] += V[row][j] * (1 / arr_w[j]) * U[col][j];
                }
            }
        }
        printf("\nSolution x by using Singular Value Decomposition: \n");
        for (row = 1; row <= n; row++)
        {
            x[row][1] = 0.0;
            for (col = 1; col <= n; col++)
                x[row][1] += Ai[row][col] * b[col][1];
            printf("%15.6f\n", x[row][1]);
        }

        printf("\nInverse of matrix A by using Singular Value Decomposition: \n");
        for (row = 1; row <= n; row++)
        {
            for (col = 1; col <= n; col++)
                printf("%15.6f", Ai[row][col]);
            printf("\n");
        }

        for (row = 1; row <= n; row++)
            det *= lu[row][row];
        printf("\nDeterminant by using LU decomposition: %15.6f\n", det);
        printf("\n");
    }

    // 동적 메모리 해제
    free_matrix(A, 1, n, 1, n);
    free_matrix(b, 1, n, 1, m);

    return 0;
}
