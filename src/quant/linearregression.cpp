#include "linearregression.h"

LinearRegression::LinearRegression()
{

}

QPair<qreal, qreal> LinearRegression::calculate(const QList<qreal> &x, const QList<qreal> &y)
{
    //TODO check x and y have the same size
    int n = x.length();

    qreal sum_x = 0.0, sum_x2=0.0, sum_y=0.0, sum_xy=0.0;

    for(int i=0; i < n; i++){
       sum_x = sum_x + x[i];
       sum_x2 = sum_x2 + x[i]*x[i];
       sum_y = sum_y + y[i];
       sum_xy = sum_xy + x[i]*y[i];
    }

    qreal a = (n*sum_xy-sum_x*sum_y)/(n*sum_x2-sum_x*sum_x);
    qreal b = (sum_y - a*sum_x)/n;
    return QPair<qreal, qreal>(a, b);
}
