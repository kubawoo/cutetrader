#ifndef DATA_DATAMANAGER_H
#define DATA_DATAMANAGER_H

#include <QSqlDatabase>
#include "model.h"
#include "utils.h"

namespace data {

class DataManager
{
public:
    DataManager(QSqlDatabase & db);
    bool createSecurity(const Security& security);
//    Security getSecurity(int id);
//    Security getSecurity(const QString & symbol);
//    bool removeSecurity(int id);
//    QList<Security> getAllSecurities();

private:

//    Security toSecurity(DbResult data);

    QSqlDatabase & _db;

};

}

#endif // DATA_DATAMANAGER_H
