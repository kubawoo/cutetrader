#include "test_correlation.h"
#include <correlation.h>
#include "../testrunner/helper.h"


void CorrelationTest::testOne()
{
    QList<qreal> x, y;
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

    qreal correlation = Correlation::calculate(x, y);
    kCompare(correlation, 0.953462589246);
}
