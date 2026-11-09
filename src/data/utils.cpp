#include "data/utils.h"
#include <QSqlError>
#include <QSqlRecord>

namespace data {

Utils::Utils()
{

}

bool Utils::execute(QSqlDatabase &db, QSqlQuery && query)
{
    qDebug() << "Executing prepared query:" << query.lastQuery();
    if (!query.exec()) {
        qDebug() << "Failed to execute" << query.lastQuery()
                 << "Error:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<DbResult> Utils::query(QSqlDatabase &db, QSqlQuery && query, bool *ok)
{
    qDebug() << "Executing prepared query:" << query.lastQuery();

    QList<DbResult> results;

    if(ok) {
        *ok = false;
    }

    if (!query.exec()) {
        qDebug() << "Failed to execute" << query.lastQuery()
                 << "Error:" << query.lastError().text();
        return results;
    }

    int columns = query.record().count();
    if(columns <= 0) {
        qDebug() << "No columns returned";
        if(ok) *ok = true;
        return results;
    }

    while (query.next()) {
        results.append(DbResult());
        DbResult & row = results.back();
        for(int i = 0; i < columns; ++i) {
            row.append(query.value(i));
        }
    }

    if(ok) {
        *ok = true;
    }
    return results;
}

bool Utils::doExecute(QSqlDatabase &db, QSqlQuery & query)
{
    bool ok = query.exec();
    if(!ok) {
        qDebug() << "Failed to execute" << query.lastQuery()
                 << "Error:" << query.lastError().text();
    }
    return ok;
}

}