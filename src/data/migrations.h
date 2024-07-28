#ifndef DATA_MIGRATIONS_H
#define DATA_MIGRATIONS_H

#include <QString>

namespace data {

class DbMigration
{
public:
    virtual ~DbMigration();
    virtual const int id() const = 0;
    virtual const QString sql() const = 0;
protected:
    DbMigration();
};


class Migration_001 : public DbMigration
{
public:
    const int id() const;
    const QString sql() const;
};


class Migration_002 : public DbMigration
{
public:
    const int id() const;
    const QString sql() const;
};


class Migration_003 : public DbMigration
{
public:
    const int id() const;
    const QString sql() const;
};

}
#endif // DATA_MIGRATIONS_H
