#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <QSqlDatabase>
#include <QSqlQuery>

namespace data {

typedef QList<QVariant> DbResult;

class Utils
{
private:
    Utils();
public:
    // Prepared statement API (prevents SQL injection)
    static bool execute(QSqlDatabase &db, QSqlQuery && query);
    static QList<DbResult> query(QSqlDatabase &db, QSqlQuery && query, bool *ok = nullptr);

private:
    static bool doExecute(QSqlDatabase &db, QSqlQuery & query);
};
}

#endif
