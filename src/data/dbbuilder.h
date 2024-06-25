#ifndef DBBUILDER_H
#define DBBUILDER_H

#include <QString>
#include <QList>

class DbMigration
{
public:
    virtual ~DbMigration();
    virtual const int order() const = 0;
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
    QList<DbMigration*> _migrations;
};



#endif // DBBUILDER_H
