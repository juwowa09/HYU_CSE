#include <stdio.h>
#include "nr.h"
#include "nrutil.h"

int main() {
	FILE* fp;
    char* data_arr[3] = {"fitdata1.dat", "fitdata2.dat", "fitdata3.dat"};
    
    float x, y, xp, yp, sum_x2, sum_xy, sum_x, sum_y2, sum_y, sum_n, sum_x_x, sum_x_y, sum_x_, sum_y_x, sum_y_y, sum_y_;
    float **mat, **vec;

    mat = matrix(1, 3, 1, 3);
	vec = matrix(1, 3, 1, 3);

    for (int i = 0; i < 3; i++) {
        if (!(fp = fopen(data_arr[i],"r")))
            nrerror("Data file error");
        sum_x2 = 0, sum_xy = 0, sum_x = 0;
        sum_y2 = 0, sum_y = 0, sum_n = 0;
        sum_x_x = 0, sum_x_y = 0, sum_x_ = 0;
        sum_y_x = 0, sum_y_y = 0, sum_y_ = 0;

        printf("========== fitdata%d.dat ==========\n", i + 1);
        while (1) {
            float x, y, x_, y_;
            fscanf(fp, "%f %f %f %f", &x, &y, &x_, &y_);
            if (feof(fp))
                break;
            sum_x2 += x * x;
            sum_xy += x * y;
            sum_x += x;
            sum_y2 += y * y;
            sum_y += y;
            sum_n += 1;
            sum_x_x += x_ * x;
            sum_x_y += x_ * y;
            sum_x_ += x_;
            sum_y_x += y_ * x;
            sum_y_y += y_ * y;
            sum_y_ += y_;
        }
        mat[1][1] = sum_x2;
        mat[1][2] = sum_xy;
        mat[1][3] = sum_x;
        mat[2][1] = sum_xy;
        mat[2][2] = sum_y2;
        mat[2][3] = sum_y;
        mat[3][1] = sum_x;
        mat[3][2] = sum_y;
        mat[3][3] = sum_n;
        vec[1][1] = sum_x_x;
        vec[2][1] = sum_x_y;
        vec[3][1] = sum_x_;
        gaussj(mat, 3, vec, 3);
        printf("a1: %f\na2: %f\na3: %f\n", vec[1][1], vec[2][1], vec[3][1]);
        mat[1][1] = sum_x2;
        mat[1][2] = sum_xy;
        mat[1][3] = sum_x;
        mat[2][1] = sum_xy;
        mat[2][2] = sum_y2;
        mat[2][3] = sum_y;
        mat[3][1] = sum_x;
        mat[3][2] = sum_y;
        mat[3][3] = sum_n;
        vec[1][1] = sum_y_x;
        vec[2][1] = sum_y_y;
        vec[3][1] = sum_y_;
        gaussj(mat, 3, vec, 3);
        printf("a4: %f\na5: %f\na6: %f\n", vec[1][1], vec[2][1], vec[3][1]);
        printf("==================================\n\n");
        fclose(fp);
    }
	return 0;
}
