#ifndef QUANT_BLACKSCHOLES_H
#define QUANT_BLACKSCHOLES_H

namespace quant {

class BlackSholesResult {
public:
    BlackSholesResult(double value, double delta);
    double value() const;
    double delta() const;

private:
    double _value;
    double _delta;
};

class BlackScholes
{
private:
    BlackScholes();
public:
    static BlackSholesResult calculate(double stockPrice, double strike, int dte, double volatility, double riskFreeRate);

private:
    static double cumulativeNormalDistribution(double x);
};

}
#endif
