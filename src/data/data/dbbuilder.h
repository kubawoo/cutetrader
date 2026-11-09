#ifndef DATA_DBBUILDER_H
#define DATA_DBBUILDER_H

#include <QString>
#include <QList>
#include <QPair>
#include <QSqlDatabase>
#include <QSharedPointer>
#include "common/idbmigration.h"

namespace data {

class DbBuilder
{
public:
    DbBuilder(QSqlDatabase & db);
    ~DbBuilder();
    bool runMigrations();
    void addMigration(QSharedPointer<common::IDbMigration> migration);

private:
    QMap<int, QString> findExecutedMigrations(bool *ok = nullptr);
    void saveMigration(const QSharedPointer<common::IDbMigration> & migration);
    void addMigrations();

    QList<QSharedPointer<common::IDbMigration>> _migrations;
    QSqlDatabase _db;
    bool _initialized;
};

}

#endif
