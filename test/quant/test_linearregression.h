#include <QTest>
#include <QObject>

class LinearRegressionTest: public QObject
{
    Q_OBJECT
private slots:
    void line();
    void line2();
    void line3();
    void complexCase();
};
