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
    auto results = Utils::query(_db, "SELECT id, symbol FROM securities;");
    QList<Security> securities;
    for(auto result: results) {
        securities.append(toSecurity(result));
    }
    return securities;
}

Security DataManager::toSecurity(DbResult &data)
{
    Security security;

    bool ok = setValue(security, &Security::setId, data[0], &QVariant::toInt);
    ok &= setValue(security, &Security::setSymbol, data[1], &QVariant::toString);
    if(!ok) {
        return Security();
    }
    return security;
}

/*
 *
                           "id INTEGER PRIMARY KEY,"
                           "security_id  INTEGER NOT NULL,"
                           "date TEXT NOT NULL,"
                           "open REAL NOT NULL,"
                           "high REAL NOT NULL,"
                           "low REAL NOT NULL,"
                           "close REAL NOT NULL,"
                           "volume INTEGER NOT NULL,"
*/

Quote DataManager::toQuote(DbResult &data)
{
    Quote quote;

    bool ok = setValue(quote, &Quote::setId, data[0], &QVariant::toInt);
    ok &= setValue(quote, &Quote::setSecurityId, data[1], &QVariant::toInt);
    ok &= setValue(quote, &Quote::setDate, data[2], &QVariant::toDate);
    ok &= setValue(quote, &Quote::setOpen, data[3], &QVariant::toDouble);
    ok &= setValue(quote, &Quote::setHigh, data[3], &QVariant::toDouble);
    ok &= setValue(quote, &Quote::setLow, data[3], &QVariant::toDouble);
    ok &= setValue(quote, &Quote::setClose, data[3], &QVariant::toDouble);
    ok &= setValue(quote, &Quote::setVolume, data[0], &QVariant::toInt);

    if(!ok) {
        return Quote();
    }
    return quote;
}




}
