#ifndef QUANT_TESTS_H
#define QUANT_TESTS_H

#include "../testrunner/multitests.h"
#include "test_blacksholes.h"
#include "test_calculate.h"
#include "test_correlation.h"
#include "test_linearregression.h"

TEST_DECLARE(LinearRegressionTest)
TEST_DECLARE(CorrelationTest);
TEST_DECLARE(CalculateTest);
TEST_DECLARE(BlackScholesTest);

#endif // QUANT_TESTS_H
