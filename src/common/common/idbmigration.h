#ifndef COMMON_IDBMIGRATION_H
#define COMMON_IDBMIGRATION_H

#include <QString>
#include <QStringList>

namespace common {

class IDbMigration
{
public:
    virtual ~IDbMigration();
    virtual const int id() const = 0;
    virtual const QStringList sql() const = 0;
    QString hash() const;
protected:
    IDbMigration();
};

}
#endif
