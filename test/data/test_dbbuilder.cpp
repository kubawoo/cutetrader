#include "test_dbbuilder.h"
#include <data/dbbuilder.h>
#include <data/utils.h>
#include <QList>


void DbBuilderTest::test()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(":memory:");
    db.open();

    data::DbBuilder dbBuilder(db);

    //check foreign keys support
    bool ok;
    QSqlQuery pragmaQuery(db);
    pragmaQuery.prepare("PRAGMA foreign_keys;");
    auto results = data::Utils::query(db, std::move(pragmaQuery), &ok);
    QVERIFY(ok);
    QCOMPARE(results[0][0].toInt(&ok), 1);
    QVERIFY(ok);

    ok = dbBuilder.runMigrations();
    QVERIFY(ok);

    QSqlQuery selectQuery(db);
    selectQuery.prepare("SELECT id FROM _migrations;");
    results = data::Utils::query(db, std::move(selectQuery));
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
    QSqlQuery updateQuery(db);
    updateQuery.prepare("UPDATE _migrations SET sha256hash = ? WHERE id=?;");
    updateQuery.bindValue(0, "123");
    updateQuery.bindValue(1, 1);
    ok = data::Utils::execute(db, std::move(updateQuery));
    QVERIFY(ok);
    ok = dbBuilder.runMigrations();
    QVERIFY(!ok);

    db.close();
}