#include "test_linearregression.h"
#include <linearregression.h>
#include "../testrunner/helper.h"

void LinearRegressionTest::line()
{
    QList<qreal> x, y;
    x.append(0); y.append(0);
    x.append(1); y.append(1);
    x.append(2); y.append(2);
    x.append(3); y.append(3);
    x.append(4); y.append(4);

    QPair<qreal,qreal> result = LinearRegression::calculate(x, y);
    QCOMPARE(result.first, 1.0);
    QCOMPARE(result.second, 0.0);
}


void LinearRegressionTest::line2()
{
    QList<qreal> x, y;
    x.append(0); y.append(0);
    x.append(1); y.append(-1);
    x.append(2); y.append(-2);
    x.append(3); y.append(-3);
    x.append(4); y.append(-4);

    QPair<qreal,qreal> result = LinearRegression::calculate(x, y);
    QCOMPARE(result.first, -1.0);
    QCOMPARE(result.second, 0.0);
}

void LinearRegressionTest::line3()
{
    QList<qreal> x, y;
    x.append(0); y.append(5);
    x.append(1); y.append(4);
    x.append(2); y.append(3);
    x.append(3); y.append(2);
    x.append(4); y.append(1);

    QPair<qreal,qreal> result = LinearRegression::calculate(x, y);
    QCOMPARE(result.first, -1.0);
    QCOMPARE(result.second, 5.0);
}


void LinearRegressionTest::complexCase()
{
    QList<qreal> x, y;

    x.append(1714); y.append(2.4);
    x.append(1664); y.append(2.52);
    x.append(1760); y.append(2.54);
    x.append(1685); y.append(2.74);
    x.append(1693); y.append(2.83);
    x.append(1670); y.append(2.91);
    x.append(1764); y.append(3);
    x.append(1764); y.append(3);
    x.append(1792); y.append(3.01);
    x.append(1850); y.append(3.01);
    x.append(1735); y.append(3.02);
    x.append(1775); y.append(3.07);
    x.append(1735); y.append(3.08);
    x.append(1712); y.append(3.08);
    x.append(1773); y.append(3.12);
    x.append(1872); y.append(3.17);
    x.append(1755); y.append(3.17);
    x.append(1674); y.append(3.17);
    x.append(1842); y.append(3.17);
    x.append(1786); y.append(3.19);
    x.append(1761); y.append(3.19);
    x.append(1722); y.append(3.19);
    x.append(1663); y.append(3.2);
    x.append(1687); y.append(3.21);
    x.append(1974); y.append(3.24);
    x.append(1826); y.append(3.28);
    x.append(1787); y.append(3.28);
    x.append(1821); y.append(3.28);
    x.append(2020); y.append(3.28);
    x.append(1794); y.append(3.28);
    x.append(1769); y.append(3.28);
    x.append(1934); y.append(3.28);
    x.append(1775); y.append(3.29);
    x.append(1855); y.append(3.29);
    x.append(1880); y.append(3.29);
    x.append(1849); y.append(3.31);
    x.append(1808); y.append(3.32);
    x.append(1954); y.append(3.34);
    x.append(1777); y.append(3.37);
    x.append(1831); y.append(3.37);
    x.append(1865); y.append(3.37);
    x.append(1850); y.append(3.38);
    x.append(1966); y.append(3.38);
    x.append(1702); y.append(3.39);
    x.append(1990); y.append(3.39);
    x.append(1925); y.append(3.4);
    x.append(1824); y.append(3.4);
    x.append(1956); y.append(3.4);
    x.append(1857); y.append(3.41);
    x.append(1979); y.append(3.41);
    x.append(1802); y.append(3.41);
    x.append(1855); y.append(3.42);
    x.append(1907); y.append(3.42);
    x.append(1634); y.append(3.42);
    x.append(1879); y.append(3.44);
    x.append(1887); y.append(3.47);
    x.append(1730); y.append(3.47);
    x.append(1953); y.append(3.47);
    x.append(1781); y.append(3.47);
    x.append(1891); y.append(3.48);
    x.append(1964); y.append(3.49);
    x.append(1808); y.append(3.49);
    x.append(1893); y.append(3.5);
    x.append(2041); y.append(3.51);
    x.append(1893); y.append(3.51);
    x.append(1832); y.append(3.52);
    x.append(1850); y.append(3.52);
    x.append(1934); y.append(3.54);
    x.append(1861); y.append(3.58);
    x.append(1931); y.append(3.58);
    x.append(1933); y.append(3.59);
    x.append(1778); y.append(3.59);
    x.append(1975); y.append(3.6);
    x.append(1934); y.append(3.6);
    x.append(2021); y.append(3.61);
    x.append(2015); y.append(3.62);
    x.append(1997); y.append(3.64);
    x.append(2020); y.append(3.65);
    x.append(1843); y.append(3.71);
    x.append(1936); y.append(3.71);
    x.append(1810); y.append(3.71);
    x.append(1987); y.append(3.73);
    x.append(1962); y.append(3.76);
    x.append(2050); y.append(3.81);

    QPair<qreal, qreal> result = LinearRegression::calculate(x, y);
    kCompare(result.first, 0.00165568805009);
    kCompare(result.second, 0.27504029966);
}


