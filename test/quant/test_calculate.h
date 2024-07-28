#ifndef TEST_CALCULATE_H
#define TEST_CALCULATE_H


#include <QTest>

class CalculateTest : public QObject
{
    Q_OBJECT
private slots:
    void testVector();
    void testQList();
    void testExpectedValue();
    void testVariance();
    void testVarianceExpectedValue();
    void testStdDev();
    void testSkew();
    void testPolynomial();
    void testCorrelation();
    void testBeta1();
    void testBeta2();
    void testBeta3();
};


#endif // TEST_CALCULATE_H
