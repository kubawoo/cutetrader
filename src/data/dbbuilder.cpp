#include "dbbuilder.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>

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
    std::sort(_migrations.begin(), _migrations.end(),
              [](DbMigration * m1, DbMigration* m2) {
        return (m1->order() < m2->order());
    });

    //TODO: check if a given migration was done

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    foreach (auto migration, _migrations) {
        qDebug() << "Running migration" << migration->order() << ": " << migration->sql();
        bool ok = query.exec(migration->sql());
        if(!ok) {
            qDebug() << "Failed to execute migration";
        } else {
            QString sql = "INSERT INTO _migrations(id, timestamp) VALUES (";
            sql.append(QString::number(migration->order()));
            sql.append(", \"");
            sql.append(QDateTime::currentDateTime().toString(Qt::ISODate));
            sql.append("\");");
            qDebug() << "Executing" << sql;
            ok = query.exec(sql);
            if(!ok) {
                qDebug() << "Failed";
            }
        }
    }
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
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);
    qDebug() << "Creating initial structure";
    bool ok = query.exec(initSql);
    if(!ok) {
        qDebug() << "Failed to create initial structure";
    }
}

DbMigration::~DbMigration()
{

}

DbMigration::DbMigration()
{
}
