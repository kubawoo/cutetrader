#include "datamanager.h"
#include "utils.h"

namespace data {

DataManager::DataManager()
    :_db(QSqlDatabase::database())
{
}

DataManager::DataManager(QSqlDatabase &db)
    :_db(db)
{

}

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
    bool ok;
    int id = data[0].toInt(&ok);
    if(!ok) {
        return Security();
    }
    QString symbol = data[1].toString();
    return Security().withId(id).withSymbol(symbol);
}




}
