#ifndef QUANT_BLACKSCHOLES_H
#define QUANT_BLACKSCHOLES_H

namespace quant {

class BlackScholes
{
public:
    BlackScholes();

    double calculate(double stockPrice, double strike, int dte, double volatility, double riskFreeRate);

private:
    double cumulativeNormalDistribution(double x);
};

}
#endif // QUANT_BLACKSCHOLES_H
