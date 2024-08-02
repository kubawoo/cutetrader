#include "test_blacksholes.h"
#include <quant/blackscholes.h>
#include "../testrunner/helper.h"

void BlackScholesTest::test()
{
    double stockPrice = 45;
    double strike = 50;
    int dte = 60;
    double volatility = 0.3;
    double riskFreeRate = 0.1;

    auto result = quant::BlackScholes::calculate(stockPrice, strike, dte, volatility, riskFreeRate);
    kCompare(result.value(), 0.7746, 4);
    kCompare(result.delta(), 0.2513, 4);
}
