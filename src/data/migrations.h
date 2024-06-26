#ifndef MIGRATIONS_H
#define MIGRATIONS_H

#include "dbbuilder.h"

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


#endif // MIGRATIONS_H
