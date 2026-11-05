#include "guiappfactory.h"
#include <twsqapi/twsclient.h>
#include <data.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

GuiAppFactory::GuiAppFactory()
{}

GuiAppFactory::~GuiAppFactory()
{}

common::ITwsClient * GuiAppFactory::createTwsClient()
{
    return new twsqapi::TwsClient();
}

void GuiAppFactory::setupDatabase(const QString & dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName.isEmpty() ? ":memory:" : dbName);

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
    }
}