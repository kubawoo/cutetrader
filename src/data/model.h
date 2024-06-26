#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QDate>

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

    qreal open() const;
    void setOpen(qreal newOpen);
    Quote& withOpen(qreal newOpen);

    qreal close() const;
    void setClose(qreal newClose);
    Quote& withClose(qreal newClose);

    qreal low() const;
    void setLow(qreal newLow);
    Quote& withLow(qreal newLow);

    qreal high() const;
    void setHigh(qreal newHigh);
    Quote& withHigh(qreal newHigh);

    long volume() const;
    void setVolume(long newVolume);
    Quote& withVolume(long newVolume);

private:
    int _id;
    QString _securityId;
    QDate _date;
    qreal _open;
    qreal _close;
    qreal _low;
    qreal _high;
    long _volume;
};

#endif // MODEL_H
