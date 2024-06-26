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
    static DbBuilder& instance();
    ~DbBuilder();
    void runMigrations();
    void addMigration(DbMigration * migration);

private:
    DbBuilder();
    QList<int> findExecutedMigrations();
    QList<DbMigration*> _migrations;
    QSqlDatabase db;
};



#endif // DBBUILDER_H
