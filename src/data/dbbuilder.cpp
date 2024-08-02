#include "data/dbbuilder.h"
#include "data/utils.h"

#include <QException>
#include <QDateTime>
#include <QPair>

namespace data {

DbBuilder::DbBuilder(QSqlDatabase & db)
    :_db(db)
{
    if(_db.driverName() == "QSQLITE") {
        qDebug() << "Enabling foreign keys";
        if(!Utils::execute(_db, "PRAGMA foreign_keys = ON;")) {
            qDebug() << "Failed to enable foreign keys";
            return;
        }
    }

    QString initSql = "CREATE TABLE IF NOT EXISTS _migrations ("
                      "id INTEGER PRIMARY KEY,"
                      "sha256hash TEXT NOT NULL,"
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
    addMigration(new Migration_004());
}


DbBuilder::~DbBuilder()
{
    foreach (auto migration, _migrations) {
        delete migration;
    }
    _migrations.clear();
}

bool DbBuilder::runMigrations()
{
    if(!_initialized) {
        qDebug() << "Not initialized, not running";
        return false;
    }

    bool ok;
    QMap<int, QString> executedMigrations = findExecutedMigrations(&ok);
    if(!ok) {
        qDebug() << "Failed to find executed migratins";
        return false;
    }

    for(auto migration : _migrations) {
        if(executedMigrations.contains(migration->id())) {
            QString hash = executedMigrations[migration->id()];
            if(hash == migration->hash()) {
                qDebug() << "Migration" << migration->id() << "already executed";
                continue;
            } else {
                qDebug() << "Migration" << migration->id() << "already executed, but hashes do not match"
                         << "Expected:" << migration->hash() << "Actual: " << hash;
                return false;
            }
        }

        qDebug() << "Running migration" << migration->id();
        for(auto sql : migration->sql()) {
            bool ok = Utils::execute(_db, sql);
            if(!ok) {
                return false;
            }
        }

        saveMigration(migration);
    }
    return true;
}

void DbBuilder::addMigration(DbMigration *migration)
{
    _migrations.append(migration);
}


QMap<int, QString> DbBuilder::findExecutedMigrations(bool *ok)
{
    QString sql = "SELECT id, sha256hash FROM _migrations;";
    auto results = Utils::query(_db, sql, ok);

    QMap<int, QString> migrations;

    for(auto row : results) {
        migrations[row[0].toInt()] = row[1].toString();
    }

    return migrations;
}

void DbBuilder::saveMigration(DbMigration *migration)
{
    QString sql = QString("INSERT INTO _migrations(id, sha256hash, timestamp) VALUES (%1, \"%2\", \"%3\");")
            .arg(QString::number(migration->id()))
            .arg(migration->hash())
            .arg(QDateTime::currentDateTime().toString(Qt::ISODateWithMs));

    QSqlQuery query(_db);
    Utils::execute(_db, sql);
}

}
