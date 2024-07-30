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
    bool ok = dbBuilder.runMigrations();
    QVERIFY(ok);

    auto results = data::Utils::query(db, "SELECT id FROM _migrations;");
    QCOMPARE(results.size(), 4);

    QList<int> ids;
    for(auto row : results) {
        ids.append(row[0].toInt());
    }

    for(int i = 0; i < results.size(); i++) {
        QCOMPARE(ids[i], i+1);
    }
    db.close();
}


