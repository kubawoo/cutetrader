#include "test_dbbuilder.h"
#include <dbbuilder.h>
#include <utils.h>
#include <QList>


void DbBuilderTest::test()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    db.open();

    data::DbBuilder dbBuilder(db);
    dbBuilder.runMigrations();

    auto results = data::Utils::query(db, "SELECT id FROM _migrations;");
    QCOMPARE(results.size(), 3);

    QList<int> ids;
    for(auto row : results) {
        ids.append(row[0].toInt());
    }

    QCOMPARE(ids[0], 1);
    QCOMPARE(ids[1], 2);
    QCOMPARE(ids[2], 3);
}


