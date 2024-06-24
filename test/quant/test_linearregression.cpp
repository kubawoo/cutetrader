#include "test_linearregression.h"
#include <linearregression.h>

void LinearRegressionTest::simpleCase()
{
    QList<qreal> x, y;
    x.append(2); y.append(5);
    x.append(5); y.append(7);
    x.append(2); y.append(6);
    x.append(8); y.append(9);
    x.append(2); y.append(7);

    QPair<qreal, qreal> result = LinearRegression::calculate(x, y);
    QVERIFY(qFuzzyCompare(result.first, 4.97916666667));
    QVERIFY(qFuzzyCompare(result.second, 0.479166666667));

}

