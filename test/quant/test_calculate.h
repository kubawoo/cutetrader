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

};


#endif // TEST_CALCULATE_H
