#include "dbbuilder.h"
#include "utils.h"

#include <QDateTime>

namespace data {

DbBuilder::DbBuilder(QSqlDatabase & db)
    :_db(db)
{
    QString initSql = "CREATE TABLE IF NOT EXISTS _migrations ("
                      "id INTEGER PRIMARY KEY,"
                      "timestamp TEXT NOT NULL"
                      ");";
    qDebug() << "Creating initial structure";
    _initialized = Utils::execute(_db, initSql);
    if(_initialized) {
        addMigrations();
    }
}

void DbBuilder::addMigrations()
{
    addMigration(new Migration_001());
    addMigration(new Migration_002());
    addMigration(new Migration_003());
}


DbBuilder::~DbBuilder()
{
    foreach (auto migration, _migrations) {
        delete migration;
    }
    _migrations.clear();
}

void DbBuilder::runMigrations()
{
    if(!_initialized) {
        qDebug() << "Not initialized, not running";
        return;
    }

    QList<int> executedMigrations = findExecutedMigrations();
    foreach (auto migration, _migrations) {
        if(executedMigrations.contains(migration->id())) {
            qDebug() << "Migration" << migration->id() << "already executed";
            continue;
        }

        qDebug() << "Running migration" << migration->id();
        bool ok = Utils::execute(_db, migration->sql());
        if(ok) {
            saveMigration(migration);
        } else {
            break;
        }
    }
}

void DbBuilder::addMigration(DbMigration *migration)
{
    _migrations.append(migration);
}


QList<int> DbBuilder::findExecutedMigrations()
{
    QString sql = "SELECT id FROM _migrations;";
    auto results = Utils::query(_db, sql);

    QList<int> ids;

    for(auto row : results) {
        ids.append(row[0].toInt());
    }

    return ids;
}

void DbBuilder::saveMigration(DbMigration *migration)
{
    QString sql = QString("INSERT INTO _migrations(id, timestamp) VALUES (%1, \"%2\");")
            .arg(QString::number(migration->id()))
            .arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs));

    QSqlQuery query(_db);
    Utils::execute(_db, sql);
}

}
