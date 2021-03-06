#include <stdlib.h>
#include <math.h>
#include "NR.h"

/**
 * @brief Cubic Spline Interpolation.
 * @note that the parameter *m is created by functions ...CubicSplineIplPara(...)
 */
static double CubicSplineIpl(int N, double(*f)(double), double x, double *a, double *m)
{
    double *h = (double *)malloc_s(N * sizeof(double));

    for (int m = 0; m < N; m++)
    {
        h[m] = a[m + 1] - a[m];
    }

    int i = 0;
    for (int j = 0; j < N; j++)
    {
        if (x < a[j + 1] && x >= a[j])
        {
            i = j;
        }
    }

    double temp;
    temp = 1 / h[i] * (m[i] / 6 * pow(a[i + 1] - x, 3) + m[i + 1] / 6 * pow(x - a[i], 3)) +
           f(a[i]) + DividedDiff(f, a + i, 2) * (x - a[i]) -
           h[i] * h[i] / 6 * ((m[i + 1] - m[i]) * (x - a[i]) / h[i] + m[i]);
    free(h);
    return temp;
}

/**
 * @brief Lagrange三次样条插值函数
 * @param N 分段区间数，即插值点数量为N + 1；
 * @param a 插值点数组指针；
 * @param f 函数；
 * @param x 变量值；
 * @return 返回样条插值函数值
 */
static double *LagrangeCubicSplineIplPara(int N, double *a, double(*f)(double))
{
    double *h = (double *)malloc_s(N * sizeof(double));			/*插值点间距*/
    double *d = (double *)malloc_s((N - 1) * sizeof(double));		/*方程组常数项*/
    double *Matd = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线*/
    double *Matc = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线上方对角线*/
    double *Mata = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线下方对角线*/

    for (int i = 0; i < N; i++)
    {
        h[i] = a[i + 1] - a[i];
    }

    //d
    d[0] = DividedDiff(f, a, 3) - (h[0] + h[1]) * DividedDiff(f, a, 4);
    for (int i = 1; i < N; i++)
    {
        d[i] = DividedDiff(f, a + i, 2) - DividedDiff(f, a + i - 1, 2);
    }
    d[N] = DividedDiff(f, a + N - 2, 3) + (h[N - 1] + h[N - 2]) * DividedDiff(f, a + N - 3, 4);
    for (int i = 0; i < N + 1; i++)
    {
        d[i] = d[i] * 6;
    }

    //Matd
    Matd[0] = 2;
    for (int i = 1; i < N; i++)
    {
        Matd[i] = (h[i - 1] + h[i]) * 2;
    }
    Matd[N] = 2;

    //Mata
    Mata[0] = 0;
    for (int i = 1; i < N; i++)
    {
        Mata[i] = h[i - 1];
    }
    Mata[N] = 1;

    //Matc
    Matc[0] = 1;
    for (int i = 1; i < N; i++)
    {
        Matc[i] = h[i];
    }
    Matc[N] = 0;

    //求解三对角方程组
    double *result = Chasing(N + 1, Matd, Matc, Mata, d);
    free(Mata);
    free(Matc);
    free(Matd);
    free(d);
    free(h);
    return result;
}

/**
 * @brief 完备三次样条插值函数
 * @param N 分段区间数，即插值点数量为N + 1；
 * @param a 插值点数组指针；
 * @param f 函数；
 * @param x 变量值；
 * @param df_a derivative at a
 * @param df_b derivative at b
 * @return 样条插值函数值
 */
static double *CompleteCubicSplineIplPara(int N, double *a, double(*f)(double), double df_a, double df_b)
{
    double *h = (double *)malloc_s(N * sizeof(double));			/*插值点间距*/
    double *d = (double *)malloc_s((N - 1) * sizeof(double));		/*方程组常数项*/
    double *Matd = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线*/
    double *Matc = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线上方对角线*/
    double *Mata = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线下方对角线*/

    for (int i = 0; i < N; i++)
    {
        h[i] = a[i + 1] - a[i];
    }

    Matd[0] = 2 * h[0];
    for (int i = 1; i < N; i++)
    {
        Matd[i] = 2 * (h[i - 1] + h[i]);
    }
    Matd[N] = 2 * h[N - 1];

    Mata[0] = 0;
    for (int i = 1; i < N + 1; i++)
    {
        Mata[i] = h[i - 1];
    }

    for (int i = 0; i < N; i++)
    {
        Matc[i] = h[i];
    }
    Matc[N] = 0;

    d[0] = DividedDiff(f, a, 2) - df_a;
    for (int i = 1; i < N; i++)
    {
        d[i] = DividedDiff(f, a + i, 2) - DividedDiff(f, a + i - 1, 2);
    }
    d[N] = df_b - DividedDiff(f, a + N - 1, 2);
    for (int i = 0; i < N + 1; i++)
    {
        d[i] = d[i] * 6;
    }

    //求解三对角方程组
    double *result = Chasing(N + 1, Matd, Matc, Mata, d);
    free(Mata);
    free(Matc);
    free(Matd);
    free(d);
    free(h);
    return result;
}

/**
 * @brief 自然三次样条插值函数
 * @param N 分段区间数，即插值点数量为N + 1；
 * @param a 插值点数组指针；
 * @param f 函数；
 * @param x 变量值；
 * @param ddf_a second order derivative at a
 * @param ddf_b second order derivative at b
 * @return 样条插值函数值
 */
static double *NatureCubicSplineIplPara(int N, double *a, double(*f)(double), double ddf_a, double ddf_b)
{
    double *d = (double *)malloc_s((N - 1) * sizeof(double));		/*方程组常数项*/
    double *Matd = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线*/
    double *Matc = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线上方对角线*/
    double *Mata = (double *)malloc_s((N - 1) * sizeof(double));	/*主对角线下方对角线*/
    double *m = (double *)malloc_s((N + 1) * sizeof(double));		/*插值参数*/
    double *temp = (double *)malloc_s((N - 1) * sizeof(double));
    double *h = (double *)malloc_s(N * sizeof(double));

    for (int i = 0; i < N; i++)
    {
        h[i] = a[i + 1] - a[i];
    }

    for (int i = 0; i < N - 1; i++)
    {
        Matd[i] = 2 * (h[i] + h[i + 1]);
        Mata[i] = h[i];
        Matc[i] = h[i + 1];
    }

    for (int i = 0; i < N - 1; i++)
    {
        d[i] = (DividedDiff(f, a + i + 1, 2) - DividedDiff(f, a + i, 2)) * 6;
    }
    d[0] = d[0] - h[0] * ddf_a;
    d[N - 2] = d[N - 2] - h[N - 1] * ddf_b;


    m[0] = ddf_a;
    temp = Chasing(N - 1, Matd, Matc, Mata, d);
    for (int i = 1; i < N; i++)
    {
        m[i] = temp[i - 1];
    }
    m[N] = ddf_b;

    free(d);
    free(Mata);
    free(Matc);
    free(Matd);
    free(h);
    free(temp);
    return m;
}

/**
 * @brief 自然三次样条插值函数
 * @param N 分段区间数，即插值点数量为N + 1
 * @param a：插值点数组
 * @param f：函数
 * @param x：变量值
 * @param ddf_a
 * @param ddf_b：两端二阶导数值
 * @return 样条插值函数值
*/
double NatureCubicSplineIpl(double (*f)(double), double ddf_a, double ddf_b, double x, int N, double *a)
{
    return CubicSplineIpl(N, f, x, a, NatureCubicSplineIplPara(N, a, f, ddf_a, ddf_b));
}

/**
 * @brief 完备三次样条插值函数
 * @param f 函数
 * @param df_a 两端导数值
 * @param df_b 
 * @param x 变量值
 * @param N 分段区间数，即插值点数量为N + 1
 * @param a 插值点数组指针
 * @returns 样条插值函数值
 */
double CompleteCubicSplineIpl(double (*f)(double), double df_a, double df_b, double x, int N, double *a)
{
    return CubicSplineIpl(N, f, x, a, CompleteCubicSplineIplPara(N, a, f, df_a, df_b));
}

/**
 * @brief Lagrange三次样条插值函数
 * @param f 函数
 * @param x 变量值
 * @param N 分段区间数，即插值点数量为N + 1
 * @param a 插值点数组指针
 * @returns 样条插值函数值
 */
double LagrangeCubicSplineIpl(double (*f)(double), double x, int N, double *a)
{
    return CubicSplineIpl(N, f, x, a, LagrangeCubicSplineIplPara(N, a, f));
}
