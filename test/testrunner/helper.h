#ifndef HELPER_H
#define HELPER_H

#include <QTest>
inline void kCompare(double x, double y, int precision = 6)
{
    QString xString = QString("%1").arg(x, 0, 'f', precision);
    QString yString = QString("%1").arg(y, 0, 'f', precision);
    QCOMPARE(xString, yString);
}

#endif // HELPER_H
