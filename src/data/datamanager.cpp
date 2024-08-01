#include "datamanager.h"
#include "utils.h"

namespace data {

DataManager::DataManager()
    :_db(QSqlDatabase::database())
{}

DataManager::DataManager(QSqlDatabase &db)
    :_db(db)
{}

bool DataManager::createSecurity(const Security &security)
{
    QString sql = QString("INSERT INTO securities(symbol) VALUES (\"%1\");")
            .arg(security.symbol());

    return Utils::execute(_db, sql);
}

Security DataManager::getSecurity(int id)
{
    QString sql = QString("SELECT id, symbol FROM securities WHERE id=%1;").arg(id);
    auto results = Utils::query(_db, sql);
    if(results.size() != 1) {
        return Security().withId(-1);
    }
    return toSecurity(results[0]);
}

Security DataManager::getSecurity(const QString &symbol)
{
    QString sql = QString("SELECT id, symbol FROM securities WHERE symbol=\"%1\";").arg(symbol);
    auto results = Utils::query(_db, sql);
    if(results.size() != 1) {
        return Security();
    }
    return toSecurity(results[0]);
}

bool DataManager::removeSecurity(int id)
{
    QString sql = QString("DELETE FROM securities WHERE id=%1;").arg(id);
    return Utils::execute(_db, sql);
}

QList<Security> DataManager::getAllSecurities()
{
    auto results = Utils::query(_db, "SELECT id, symbol FROM securities ORDER BY symbol;");
    QList<Security> securities;
    for(auto result: results) {
        securities.append(toSecurity(result));
    }
    return securities;
}

bool DataManager::createQuote(const Quote &quote)
{
    QString sql = QString("INSERT INTO quotes(security_id, date, open, high, low, close, volume) "
                          "VALUES (%1, \"%2\", %3, %4, %5, %6, %7);")
            .arg(quote.securityId())
            .arg(quote.date().toString(Qt::DateFormat::ISODate))
            .arg(quote.open())
            .arg(quote.high())
            .arg(quote.low())
            .arg(quote.close())
            .arg(quote.volume());

    return Utils::execute(_db, sql);
}

QList<Quote> DataManager::getQuotes(const Security &security)
{
    QString sql = QString("SELECT id, security_id, date, open, high, low, close, volume "
                          "FROM quotes WHERE security_id=%1 ORDER BY date;")
            .arg(security.id());

    auto results = Utils::query(_db, sql);

    QList<Quote> quotes;
    for(auto r : results) {
        quotes.append(toQuote(r));
    }
    return quotes;
}

Security DataManager::toSecurity(DbResult &data)
{
    Security security;
    bool ok = data.size() == 2;
    ok && (ok &= setValue(security, &Security::setId, data[0], &QVariant::toInt));
    ok && (ok &= setValue(security, &Security::setSymbol, data[1], &QVariant::toString));

    if(!ok) {
        return Security();
    }
    return security;
}

Quote DataManager::toQuote(DbResult &data)
{
    Quote quote;

    bool ok = data.size() == 8;
    ok && (ok &= setValue(quote, &Quote::setId, data[0], &QVariant::toInt));
    ok && (ok &= setValue(quote, &Quote::setSecurityId, data[1], &QVariant::toInt));
    ok && (ok &= setValue(quote, &Quote::setDate, data[2], &QVariant::toDate));
    ok && (ok &= setValue(quote, &Quote::setOpen, data[3], &QVariant::toDouble));
    ok && (ok &= setValue(quote, &Quote::setHigh, data[4], &QVariant::toDouble));
    ok && (ok &= setValue(quote, &Quote::setLow, data[5], &QVariant::toDouble));
    ok && (ok &= setValue(quote, &Quote::setClose, data[6], &QVariant::toDouble));
    ok && (ok &= setValue(quote, &Quote::setVolume, data[7], &QVariant::toInt));

    if(!ok) {
        return Quote();
    }
    return quote;
}




}
