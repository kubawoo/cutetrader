#include "quant/blackscholes.h"
#include <cmath>
#include <vector>
#include "quant/calculate.h"


namespace quant {

BlackSholesResult::BlackSholesResult(double value, double delta)
    : _value(value),
    _delta(delta)
{}

double BlackSholesResult::value() const
{
    return _value;
}

double BlackSholesResult::delta() const
{
    return _delta;
}


BlackScholes::BlackScholes()
{

}

BlackSholesResult BlackScholes::calculate(double p, double s, int dte, double v, double r)
{
    double t = dte / 365.0;
    double d1 = (log(p/s) + (r + pow(v, 2)/2) * t) / (v * sqrt(t));
    double d2 = d1 - v * sqrt(t);

    double nd1 = cumulativeNormalDistribution(d1);
    double nd2 = cumulativeNormalDistribution(d2);

    double value = p * nd1 - s*exp(-r*t) * nd2;
    return BlackSholesResult(value, nd1);
}

double BlackScholes::cumulativeNormalDistribution(double x)
{
    std::vector<double> a{1.330274429, -1.821255978, 1.781477937, -0.356563782, 0.31938153, 0};
    double absX = fabs(x);
    double y = 1.0 / (1.0 + 0.2316419 * absX);
    double z = 0.3989423 * exp(-absX*absX/2);
    double w = 1.0 - z * Calculate::polynomial(a, y);

    return x < 0 ? 1.0 - w : w;
}



}
