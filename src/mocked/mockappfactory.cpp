#include <QSqlDatabase>
#include <memory>

#include "mocked/mockappfactory.h"
#include "mocked/twsclientmock.h"
#include "mocked/mockmigrations.h"

namespace mocked {

MockAppFactory::MockAppFactory()
{
    qDebug() << "Constructing mocked::MockAppFactory" << this;
}

MockAppFactory::~MockAppFactory()
{
    qDebug() << "Destroying mocked::MockAppFactory" << this;
}

common::ITwsClient *MockAppFactory::createTwsClient()
{
    return new TwsClientMock;
}

void MockAppFactory::setupDatabase(const QString & dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName.isEmpty() ? ":memory:" : dbName);
    db.open();
}

QList<std::shared_ptr<common::IDbMigration>> MockAppFactory::extraMigrations()
{
    return {
        std::make_shared<MockMigration_1001>()
    };
}

}