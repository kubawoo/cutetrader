#include "utils.h"
#include <QSqlError>
#include <QSqlRecord>

namespace data {

Utils::Utils()
{

}

bool Utils::execute(QSqlDatabase &db, const QString &sql)
{
    qDebug() << "Executing SQL"<< sql;
    QSqlQuery query(db);
    bool ok = doExecute(db, query, sql);
    query.finish();
    return ok;
}

QList<DbResult> Utils::query(QSqlDatabase &db, const QString &sql)
{
    qDebug() << "Executing SQL"<< sql;

    QList<DbResult> results;
    QSqlQuery query(db);

    bool ok = doExecute(db, query, sql);
    if(!ok) {
        return results;
    }

    int columns = query.record().count();
    if(columns <= 0) {
        qDebug() << "No columns returned for" <<sql;
        return results;
    }

    while (query.next()) {
        results.append(DbResult());
        DbResult & row = results.back();
        for(int i = 0; i < columns; ++i) {
            row.append(query.value(i));
        }
    }

    query.finish();

    return results;
}

bool Utils::doExecute(QSqlDatabase &db, QSqlQuery & query, const QString & sql)
{
    bool ok = query.exec(sql);
    if(!ok) {
        qDebug() << "Failed to execute" << query.lastQuery()
                 << "Error:" << query.lastError().text();
    }
    return ok;
}

}
