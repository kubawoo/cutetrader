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

    //check foreign keys support
    bool ok;
    auto results = data::Utils::query(db, "PRAGMA foreign_keys;", &ok);
    QVERIFY(ok);
    QCOMPARE(results[0][0].toInt(&ok), 1);
    QVERIFY(ok);

    ok = dbBuilder.runMigrations();
    QVERIFY(ok);

    results = data::Utils::query(db, "SELECT id FROM _migrations;");
    QCOMPARE(results.size(), 4);

    QList<int> ids;
    for(auto row : results) {
        ids.append(row[0].toInt(&ok));
        QVERIFY(ok);
    }

    for(int i = 0; i < results.size(); i++) {
        QCOMPARE(ids[i], i+1);
    }

    //mess up with migrations hash
    ok = data::Utils::execute(db, "UPDATE _migrations SET sha256hash = \"123\" WHERE id=1;");
    QVERIFY(ok);
    ok = dbBuilder.runMigrations();
    QVERIFY(!ok);

    db.close();
}


