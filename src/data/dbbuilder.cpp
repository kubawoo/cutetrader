#include "dbbuilder.h"

#include <QSqlError>
#include <QDateTime>

DbMigration::~DbMigration()
{}

DbMigration::DbMigration()
{}

DbBuilder::DbBuilder(QSqlDatabase & db)
    :_db(db)
{
    QString initSql = "CREATE TABLE IF NOT EXISTS _migrations ("
                      "id INTEGER PRIMARY KEY,"
                      "timestamp TEXT NOT NULL"
                      ");";
    qDebug() << "Creating initial structure";
    _initialized = executeSql(initSql);
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
        bool ok = executeSql(migration->sql());
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
    QSqlQuery query(_db);
    query.exec(sql);

    QList<int> ids;

    while (query.next()) {
        int id = query.value(0).toInt();
        ids.append(id);
    }

    return ids;
}

void DbBuilder::saveMigration(DbMigration *migration)
{
    QString sql = "INSERT INTO _migrations(id, timestamp) VALUES (";
    sql.append(QString::number(migration->id()));
    sql.append(", \"");
    sql.append(QDateTime::currentDateTime().toString(Qt::ISODateWithMs));
    sql.append("\");");
    QSqlQuery query(_db);
    executeSql(sql);
}

bool DbBuilder::executeSql(const QString &sql)
{
    qDebug() << "Executing SQL"<< sql;
    QSqlQuery query(_db);
    bool ok = query.exec(sql);
    if(!ok) {
        qDebug() << "Failed to execute" << query.lastQuery()
                 << "Error:" << query.lastError().text();
    }
    query.finish();
    return ok;
}
