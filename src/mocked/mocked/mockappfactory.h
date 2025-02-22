#ifndef MOCKED_MOCKAPPFACTORY_H
#define MOCKED_MOCKAPPFACTORY_H

#include <common.h>

namespace mocked {

class MockAppFactory : public common::IAppFactory
{
public:
    MockAppFactory();
    ~MockAppFactory();

    common::ITwsClient * createTwsClient() override;
    void setupDatabase() override;
    QList<common::IDbMigration *> extraMigrations() override;
};
}

#endif // MOCKED_MOCKAPPFACTORY_H
