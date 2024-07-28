#include "test_calculate.h"
#include <calculate.h>
#include "../testrunner/helper.h"


void CalculateTest::testVector()
{
    std::vector<double> x;
    x.push_back(15.0);
    x.push_back(18.2);
    x.push_back(21.3);
    x.push_back(24.5);
    x.push_back(27.0);

    double sum = quant::Calculate::sum(x);
    kCompare(sum, 106.0);

    double mean = quant::Calculate::mean(x);
    kCompare(mean, 21.2);
}


void CalculateTest::testQList()
{
    QList<double> x;
    x.append(15.0);
    x.append(18.2);
    x.append(21.3);
    x.append(24.5);
    x.append(27.0);


    double sum = quant::Calculate::sum(x);
    kCompare(sum, 106.0);

    double mean = quant::Calculate::mean(x);
    kCompare(mean, 21.2);
}

void CalculateTest::testExpectedValue()
{
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);

    QList<double> p;
    p.append(0.1);
    p.append(0.25);
    p.append(0.3);
    p.append(0.2);
    p.append(0.15);

    double expected = quant::Calculate::expectedValue(x, p);
    kCompare(expected, 3.05);

}

void CalculateTest::testVariance()
{
    // one dice
    std::vector<int> x;
    x.push_back(1);
    x.push_back(2);
    x.push_back(3);
    x.push_back(4);
    x.push_back(5);
    x.push_back(6);

    double variance = quant::Calculate::variance(x);
    kCompare(variance, 2.916667);

    // two dices
    std::vector<int> y;
    y.push_back(2); //1+1
    y.push_back(3); //1+2
    y.push_back(4); //1+3
    y.push_back(5); //1+4
    y.push_back(6); //1+5
    y.push_back(7); //1+6

    y.push_back(3); //2+1
    y.push_back(4); //2+2
    y.push_back(5); //2+3
    y.push_back(6); //2+4
    y.push_back(7); //2+5
    y.push_back(8); //2+6

    y.push_back(4); //3+1
    y.push_back(5); //3+2
    y.push_back(6); //3+3
    y.push_back(7); //3+4
    y.push_back(8); //3+5
    y.push_back(9); //3+6

    y.push_back(5); //4+1
    y.push_back(6); //4+2
    y.push_back(7); //4+3
    y.push_back(8); //4+4
    y.push_back(9); //4+5
    y.push_back(10); //4+6

    y.push_back(6); //5+1
    y.push_back(7); //5+2
    y.push_back(8); //5+3
    y.push_back(9); //5+4
    y.push_back(10); //5+5
    y.push_back(11); //5+6

    y.push_back(7); //6+1
    y.push_back(8); //6+2
    y.push_back(9); //6+3
    y.push_back(10); //6+4
    y.push_back(11); //6+5
    y.push_back(12); //6+6


    variance = quant::Calculate::variance(y);
    kCompare(variance, 5.833333);
}

void CalculateTest::testVarianceExpectedValue()
{
    std::vector<int> x{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<double> p{1/36.0, 2/36.0, 3/36.0, 4/36.0, 5/36.0, 6/36.0, 5/36.0, 4/36.0, 3/36.0, 2/36.0, 1/36.0};

    double variance = quant::Calculate::variance(x, p);
    kCompare(variance, 5.833333);
}

void CalculateTest::testStdDev()
{
    std::vector<int> x{2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<double> p{1/36.0, 2/36.0, 3/36.0, 4/36.0, 5/36.0, 6/36.0, 5/36.0, 4/36.0, 3/36.0, 2/36.0, 1/36.0};

    double stdDev = quant::Calculate::stdDev(x, p);
    kCompare(stdDev, 2.41522938);
}

void CalculateTest::testSkew()
{
    double skew = quant::Calculate::skew(std::vector<double> {1, 2, 3, 4, 5, 6});
    kCompare(skew, 0.0);
}

void CalculateTest::testPolynomial()
{
    std::vector<int> a{2, -6, 2, -1};
    double result = quant::Calculate::polynomial(a, 3.0);
    QCOMPARE(result, 5);
}

void CalculateTest::testCorrelation()
{
    QList<double> x, y;
    x.append(15.0);
    x.append(18.0);
    x.append(21.0);
    x.append(24.0);
    x.append(27.0);

    y.append(25.0);
    y.append(25.0);
    y.append(27.0);
    y.append(31.0);
    y.append(32.0);

    double correlation = quant::Calculate::correlation(x, y);
    kCompare(correlation, 0.953462589246);
}

void CalculateTest::testBeta1()
{
    QList<qreal> x, y;
    x.append(0); y.append(0);
    x.append(1); y.append(1);
    x.append(2); y.append(2);
    x.append(3); y.append(3);
    x.append(4); y.append(4);

    double result = quant::Calculate::beta(x, y);
    QCOMPARE(result, 1.0);
}

void CalculateTest::testBeta2()
{
    QList<qreal> x, y;
    x.append(0); y.append(0);
    x.append(1); y.append(-1);
    x.append(2); y.append(-2);
    x.append(3); y.append(-3);
    x.append(4); y.append(-4);

    double result = quant::Calculate::beta(x, y);
    QCOMPARE(result, -1.0);
}

void CalculateTest::testBeta3()
{
    QList<qreal> x, y;
    x.append(0); y.append(5);
    x.append(1); y.append(4);
    x.append(2); y.append(3);
    x.append(3); y.append(2);
    x.append(4); y.append(1);

    double result = quant::Calculate::beta(x, y);
    QCOMPARE(result, -1.0);
}


