#include "NR.h"

/**
 * @brief Classic Runge-Kutta Method
 */
double *ClassicRungeKutta(double(*f)(double, double), double a, double b, double y0, int N)
{
    double k[4];
    double h = (b - a) / N;
    double x = a;
    double y = y0;
    double* result = (double*)malloc_s((N + 1) * sizeof(double));
    result[0] = y0;

    for(int i = 0; i < N; i++)
    {
        k[0] = f(x, y);
        k[1] = f(x + h / 2, y + h * k[0] / 2);
        k[2] = f(x + h / 2, y + h * k[1] / 2);
        k[3] = f(x + h, y + h * k[2]);
        y += h / 6 * (k[0] + 2 * k[1] + 2 * k[2] + k[3]);
        x += h;
        result[i + 1] = y;
    }

    return result;
}

