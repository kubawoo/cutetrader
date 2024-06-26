#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QString>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>

class DbMigration
{
public:
    virtual ~DbMigration();
    virtual const int id() const = 0;
    virtual const QString sql() const = 0;
protected:
    DbMigration();
};

class DbBuilder
{
public:
    DbBuilder(QSqlDatabase & db);
    ~DbBuilder();
    void runMigrations();
    void addMigration(DbMigration * migration);

private:
    QList<int> findExecutedMigrations();
    void saveMigration(DbMigration * migration);
    bool executeSql(const QString & sql);

    QList<DbMigration*> _migrations;
    QSqlDatabase _db;
    bool _initialized;
};



#endif // DBBUILDER_H
