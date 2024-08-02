#ifndef DATA_MIGRATIONS_H
#define DATA_MIGRATIONS_H

#include <QString>
#include <QStringList>

namespace data {

class DbMigration
{
public:
    virtual ~DbMigration();
    virtual const int id() const = 0;
    virtual const QStringList sql() const = 0;
    QString hash() const;
protected:
    DbMigration();
};


class Migration_001 : public DbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};


class Migration_002 : public DbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};


class Migration_003 : public DbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};

class Migration_004 : public DbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};

}
#endif
