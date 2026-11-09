#include "common/iappfactory.h"

common::IAppFactory::IAppFactory()
{

}

common::IAppFactory::~IAppFactory()
{

}

QList<QSharedPointer<common::IDbMigration>> common::IAppFactory::extraMigrations()
{
    return {};
}
