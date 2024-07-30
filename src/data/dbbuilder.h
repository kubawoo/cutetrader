#ifndef DATA_DBBUILDER_H
#define DATA_DBBUILDER_H

#include <QString>
#include <QList>
#include <QPair>
#include <QSqlDatabase>
#include "migrations.h"
namespace data {

class DbBuilder
{
public:
    DbBuilder(QSqlDatabase & db);
    ~DbBuilder();
    bool runMigrations();

private:
    QMap<int, QString> findExecutedMigrations(bool *ok = nullptr);
    void saveMigration(DbMigration * migration);
    void addMigrations();
    void addMigration(DbMigration * migration);

    QList<DbMigration*> _migrations;
    QSqlDatabase _db;
    bool _initialized;
};

}

#endif // DATA_DBBUILDER_H
