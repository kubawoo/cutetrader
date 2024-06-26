#include "dbbuilder.h"

#include <QDateTime>

DbMigration::~DbMigration()
{}

DbMigration::DbMigration()
{}

DbBuilder &DbBuilder::instance()
{
    static DbBuilder INSTANCE;
    return INSTANCE;
}

DbBuilder::~DbBuilder()
{
    foreach (auto migration, _migrations) {
        delete migration;
    }
}

void DbBuilder::runMigrations()
{
    QList<int> executedMigrations = findExecutedMigrations();
    foreach (auto migration, _migrations) {
        if(executedMigrations.contains(migration->id())) {
            qDebug() << "Migration" << migration->id() << "already executed";
        } else {
            QSqlQuery query(db);
            qDebug() << "Running migration" << migration->id() << ": " << migration->sql();
            bool ok = query.exec(migration->sql());
            if(!ok) {
                qDebug() << "Failed to execute migration";
            } else {
                QString sql = "INSERT INTO _migrations(id, timestamp) VALUES (";
                sql.append(QString::number(migration->id()));
                sql.append(", \"");
                sql.append(QDateTime::currentDateTime().toString(Qt::ISODateWithMs));
                sql.append("\");");
                qDebug() << "Executing" << sql;
                ok = query.exec(sql);
                if(!ok) {
                    qDebug() << "Failed";
                }
            }
        }
        delete migration;
    }
    _migrations.clear();
}

void DbBuilder::addMigration(DbMigration *migration)
{
    _migrations.append(migration);
}

DbBuilder::DbBuilder()
{
    QString initSql = "CREATE TABLE IF NOT EXISTS _migrations ("
            "id INTEGER PRIMARY KEY,"
            "timestamp TEXT NOT NULL"
            ");";
    db = QSqlDatabase::database();
    QSqlQuery query(db);
    qDebug() << "Creating initial structure";
    bool ok = query.exec(initSql);
    if(!ok) {
        qDebug() << "Failed to create initial structure";
    }
}

QList<int> DbBuilder::findExecutedMigrations()
{
    QString sql = "SELECT id FROM _migrations;";
    QSqlQuery query(db);
    query.exec(sql);

    QList<int> ids;

    while (query.next()) {
        int id = query.value(0).toInt();
        ids.append(id);
    }

    return ids;
}



