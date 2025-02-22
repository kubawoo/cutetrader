#include "appfactory.h"

#include <QSqlDatabase>
#include <twsclient.h>

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
    return new twsclient::TwsClient;
}

void AppFactory::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cutetrader.db");
    db.open();
}
