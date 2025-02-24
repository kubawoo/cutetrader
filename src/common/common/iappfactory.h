#ifndef COMMON_IAPPFACTORY_H
#define COMMON_IAPPFACTORY_H

#include "itwsclient.h"

#include <QList>
#include "idbmigration.h"

namespace common {

class IAppFactory
{
public:
    IAppFactory();
    virtual ~IAppFactory();

    virtual ITwsClient * createTwsClient() = 0;
    virtual void setupDatabase(const QString & dbName) = 0;
    virtual QList<IDbMigration *> extraMigrations();
};
}

#endif // COMMON_IAPPFACTORY_H

