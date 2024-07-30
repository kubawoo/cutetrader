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
    static bool execute(QSqlDatabase &db, const QString & sql);
    static QList<DbResult> query(QSqlDatabase &db, const QString & sql, bool *ok = nullptr);

private:
    static bool doExecute(QSqlDatabase &db, QSqlQuery & query, const QString & sql);
};
}

#endif // DATA_UTILS_H
