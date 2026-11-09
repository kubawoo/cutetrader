#include "data/dbbuilder.h"
#include "data/utils.h"
#include "data/migrations.h"

#include <QException>
#include <QDateTime>
#include <QPair>

namespace data {

DbBuilder::DbBuilder(QSqlDatabase & db)
    :_db(db), _initialized(false)
{
    if(_db.driverName() == "QSQLITE") {
        qDebug() << "Enabling foreign keys";
        QSqlQuery pragmaQuery(_db);
        pragmaQuery.prepare("PRAGMA foreign_keys = ON;");
        if(!Utils::execute(_db, std::move(pragmaQuery))) {
            qDebug() << "Failed to enable foreign keys";
            return;
        }
    }

    QString initSql = "CREATE TABLE IF NOT EXISTS _migrations ("
                      "id INTEGER PRIMARY KEY,"
                      "sha256hash TEXT NOT NULL,"
                      "timestamp TEXT NOT NULL"
                      ");";

    QSqlQuery createQuery(_db);
    createQuery.prepare(initSql);

    qDebug() << "Creating initial structure";
    _initialized = Utils::execute(_db, std::move(createQuery));
    if(_initialized) {
        addMigrations();
    }
}

void DbBuilder::addMigrations()
{
    addMigration(std::make_shared<Migration_001>());
    addMigration(std::make_shared<Migration_002>());
    addMigration(std::make_shared<Migration_003>());
    addMigration(std::make_shared<Migration_004>());
}


DbBuilder::~DbBuilder()
{
    // std::shared_ptr automatically cleans up
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
            QSqlQuery migrationQuery(_db);
            migrationQuery.prepare(sql);
            bool ok = Utils::execute(_db, std::move(migrationQuery));
            if(!ok) {
                return false;
            }
        }

        saveMigration(migration);
    }
    return true;
}

void DbBuilder::addMigration(std::shared_ptr<common::IDbMigration> migration)
{
    _migrations.append(migration);
}


QMap<int, QString> DbBuilder::findExecutedMigrations(bool *ok)
{
    QSqlQuery query(_db);
    query.prepare("SELECT id, sha256hash FROM _migrations;");
    auto results = Utils::query(_db, std::move(query), ok);

    QMap<int, QString> migrations;

    for(auto row : results) {
        migrations[row[0].toInt()] = row[1].toString();
    }

    return migrations;
}

void DbBuilder::saveMigration(const std::shared_ptr<common::IDbMigration> &migration)
{
    QSqlQuery query(_db);
    query.prepare("INSERT INTO _migrations(id, sha256hash, timestamp) VALUES (?, ?, ?);");
    query.bindValue(0, migration->id());
    query.bindValue(1, migration->hash());
    query.bindValue(2, QDateTime::currentDateTime().toString(Qt::ISODateWithMs));

    Utils::execute(_db, std::move(query));
}

}