#ifndef CORRELATION_H
#define CORRELATION_H

#include <QList>

class Correlation
{
private:
    Correlation();

public:
    static qreal calculate(const QList<qreal> & x, const QList<qreal> & y);
};

#endif // CORRELATION_H
