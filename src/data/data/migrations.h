#ifndef DATA_MIGRATIONS_H
#define DATA_MIGRATIONS_H

#include <QString>
#include <QStringList>
#include <common/idbmigration.h>

namespace data {


class Migration_001 : public common::IDbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};


class Migration_002 : public common::IDbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};


class Migration_003 : public common::IDbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};

class Migration_004 : public common::IDbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};

}
#endif
