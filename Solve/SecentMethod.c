#include <math.h>
#include "NR.h"

/**
 * @brief Secent method to solve a equation
 * @param x0 the first point
 * @param x1 the second point
 */
double SecentMethod(double(*f)(double), double x0, double x1, double eps)
{
    double y0 = f(x0);
    double y1 = f(x1);
    double p;
    while(1)
    {
        p = x1 - y1 * (x1 - x0) / (y1 - y0);
        if(fabs(p - x1) < eps)
        {
            return p;
        }
        else
        {
            x0 = x1;
            x1 = p;
            y0 = y1;
            y1= f(x1);
        }
    }
}
