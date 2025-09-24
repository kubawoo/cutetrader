#include "appfactory.h"

#include <QSqlDatabase>
#include <twsqapi.h>

AppFactory::AppFactory()
{
    qDebug() << "Constructing AppFactory" << this;
}

AppFactory::~AppFactory()
{
    qDebug() << "Destroying AppFactory" << this;
}

common::ITwsClient *AppFactory::createTwsClient()
{
    return new twsqapi::TwsClient;
}

void AppFactory::setupDatabase(const QString & dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName.isEmpty() ? "cutetrader.db" : dbName);
    db.open();
}
