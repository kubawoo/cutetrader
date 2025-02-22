#ifndef DATA_MIGRATIONS_H
#define DATA_MIGRATIONS_H

#include <QString>
#include <QStringList>
#include <common/idbmigration.h>

namespace mocked {


class MockMigration_1001 : public common::IDbMigration
{
public:
    const int id() const;
    const QStringList sql() const;
};


}
#endif
