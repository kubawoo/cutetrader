#ifndef DATA_MODEL_H
#define DATA_MODEL_H

#include <QString>
#include <QDate>

namespace data {


class Security
{
public:
    Security();
    int id() const;
    void setId(int id);
    Security & withId(int id);

    QString symbol() const;
    void setSymbol(const QString & symbol);
    Security & withSymbol(const QString & symbol);

private:
    int _id;
    QString _symbol;
};


class Quote
{
public:
    Quote();
    int id() const;
    void setId(int newId);
    Quote& withId(int newId);

    QString securityId() const;
    void setSecurityId(const QString &newSecurityId);
    Quote& withSecurityId(const QString &newSecurityId);

    QDate date() const;
    void setDate(const QDate &newDate);
    Quote& withDate(const QDate &newDate);

    double open() const;
    void setOpen(double newOpen);
    Quote& withOpen(double newOpen);

    double close() const;
    void setClose(double newClose);
    Quote& withClose(double newClose);

    double low() const;
    void setLow(double newLow);
    Quote& withLow(double newLow);

    double high() const;
    void setHigh(double newHigh);
    Quote& withHigh(double newHigh);

    long volume() const;
    void setVolume(long newVolume);
    Quote& withVolume(long newVolume);

private:
    int _id;
    QString _securityId;
    QDate _date;
    double _open;
    double _close;
    double _low;
    double _high;
    long _volume;
};

}
#endif // DATA_MODEL_H
