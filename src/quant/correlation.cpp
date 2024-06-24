#include "correlation.h"

Correlation::Correlation()
{

}

qreal Correlation::calculate(const QList<qreal> &x, const QList<qreal> &y)
{
    qreal sum_X = 0, sum_Y = 0, sum_XY = 0;
    qreal squareSum_X = 0, squareSum_Y = 0;
    int n = x.length();

    for (int i = 0; i < n; i++){
       sum_X = sum_X + x[i];
       sum_Y = sum_Y + y[i];
       sum_XY = sum_XY + x[i] * y[i];
       squareSum_X = squareSum_X + x[i] * x[i];
       squareSum_Y = squareSum_Y + y[i] * y[i];
    }
    qreal corr = (n * sum_XY - sum_X * sum_Y) / sqrt((n * squareSum_X - sum_X * sum_X) * (n * squareSum_Y - sum_Y * sum_Y));
    return corr;
}
