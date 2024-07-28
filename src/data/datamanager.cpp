#include "datamanager.h"
#include "utils.h"

namespace data {

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




}
