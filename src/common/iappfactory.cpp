#include "common/iappfactory.h"

common::IAppFactory::IAppFactory()
{

}

common::IAppFactory::~IAppFactory()
{

}

QList<common::IDbMigration *> common::IAppFactory::extraMigrations()
{
    return {};
}
