#include "common/iappfactory.h"

common::IAppFactory::IAppFactory()
{

}

common::IAppFactory::~IAppFactory()
{

}

QList<std::shared_ptr<common::IDbMigration>> common::IAppFactory::extraMigrations()
{
    return {};
}