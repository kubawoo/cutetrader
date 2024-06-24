#ifndef LINEARREGRESSION_H
#define LINEARREGRESSION_H

#include <QPair>
#include <QList>


class LinearRegression
{
private:
    LinearRegression();

public:
    static QPair<qreal, qreal> calculate(const QList<qreal> & x, const QList<qreal> & y);

};

#endif // LINEARREGRESSION_H
