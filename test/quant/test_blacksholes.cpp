#include "test_blacksholes.h"
#include <blackscholes.h>
#include "../testrunner/helper.h"

void BlackScholesTest::test()
{
    double stockPrice = 45;
    double strike = 50;
    int dte = 60;
    double volatility = 0.3;
    double riskFreeRate = 0.1;

    quant::BlackScholes model;

    double modelValue = model.calculate(stockPrice, strike, dte, volatility, riskFreeRate);
    kCompare(modelValue, 0.7746, 4);
}
