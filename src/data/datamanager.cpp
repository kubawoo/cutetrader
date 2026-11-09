#include "data/datamanager.h"
#include "data/utils.h"

namespace data {

DataManager::DataManager()
    :_db(QSqlDatabase::database())
{}

DataManager::DataManager(QSqlDatabase &db)
    :_db(db)
{}

void DataManager::close()
{
    _db.close();
}

bool DataManager::createSecurity(const Security &security)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO securities(symbol, contractId) VALUES (?, ?);");
    query.bindValue(0, security.symbol());
    query.bindValue(1, security.contractId());

    return Utils::execute(_db, std::move(query));
}

Security DataManager::getSecurity(int id)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, symbol, contractId FROM securities WHERE id=?;");
    query.bindValue(0, id);
    auto results = Utils::query(_db, std::move(query));
    if(results.size() != 1) {
        return Security().withId(-1);
    }
    return toSecurity(results[0]);
}

Security DataManager::getSecurity(const QString &symbol)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, symbol, contractId FROM securities WHERE symbol=?;");
    query.bindValue(0, symbol);
    auto results = Utils::query(_db, std::move(query));
    if(results.size() != 1) {
        return Security();
    }
    return toSecurity(results[0]);
}

bool DataManager::removeSecurity(int id)
{
    QSqlQuery query(_db);
    query.prepare("DELETE FROM securities WHERE id=?;");
    query.bindValue(0, id);
    return Utils::execute(_db, std::move(query));
}

QList<Security> DataManager::getAllSecurities()
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, symbol, contractId FROM securities ORDER BY symbol;");
    auto results = Utils::query(_db, std::move(query));
    QList<Security> securities;
    for(auto result: results) {
        securities.append(toSecurity(result));
    }
    return securities;
}

bool DataManager::createQuote(const Quote &quote)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO quotes(security_id, date, open, high, low, close, volume) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?);");
    query.bindValue(0, quote.securityId());
    query.bindValue(1, quote.date().toString(Qt::DateFormat::ISODate));
    query.bindValue(2, quote.open());
    query.bindValue(3, quote.high());
    query.bindValue(4, quote.low());
    query.bindValue(5, quote.close());
    query.bindValue(6, quote.volume());

    return Utils::execute(_db, std::move(query));
}

QList<Quote> DataManager::getQuotes(const Security &security)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, security_id, date, open, high, low, close, volume "
                  "FROM quotes WHERE security_id=? ORDER BY date;");
    query.bindValue(0, security.id());
    return doGetQuotes(std::move(query));
}

QList<Quote> DataManager::getQuotes(const Security &security, const QDate &from)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, security_id, date, open, high, low, close, volume "
                  "FROM quotes WHERE security_id=? AND date >= ? ORDER BY date;");
    query.bindValue(0, security.id());
    query.bindValue(1, from.toString(Qt::DateFormat::ISODate));
    return doGetQuotes(std::move(query));
}

QList<Quote> DataManager::getQuotes(const Security &security, const QDate &from, const QDate &to)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, security_id, date, open, high, low, close, volume "
                  "FROM quotes WHERE security_id=? AND date >= ? AND date < ? ORDER BY date;");
    query.bindValue(0, security.id());
    query.bindValue(1, from.toString(Qt::DateFormat::ISODate));
    query.bindValue(2, to.toString(Qt::DateFormat::ISODate));
    return doGetQuotes(std::move(query));
}

Security DataManager::toSecurity(DbResult &data)
{
    Security security;
    bool ok = data.size() == 3;
    ok && (ok &= setValue(security, &Security::setId, data[0], &QVariant::toInt));
    ok && (ok &= setValue(security, &Security::setSymbol, data[1], &QVariant::toString));
    ok && (ok &= setValue(security, &Security::setContractId, data[2], &QVariant::toInt));

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

QList<Quote> DataManager::doGetQuotes(QSqlQuery &&query)
{
    auto results = Utils::query(_db, std::move(query));

    QList<Quote> quotes;
    for(auto r : results) {
        quotes.append(toQuote(r));
    }
    return quotes;
}

}