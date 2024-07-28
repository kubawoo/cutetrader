#include "migrations.h"

namespace data {

DbMigration::~DbMigration()
{}

DbMigration::DbMigration()
{}

const int Migration_001::id() const
{
    return 1;
}

const QString Migration_001::sql() const
{
    return "CREATE TABLE securities ("
            "id INTEGER PRIMARY KEY,"
            "symbol TEXT NOT NULL UNIQUE"
            ");";
}

const int Migration_002::id() const
{
    return 2;
}

const QString Migration_002::sql() const
{
    return "CREATE TABLE quotes ("
            "id INTEGER PRIMARY KEY,"
            "security_id  INTEGER NOT NULL,"
            "date TEXT NOT NULL,"
            "open REAL NOT NULL,"
            "high REAL NOT NULL,"
            "low REAL NOT NULL,"
            "close REAL NOT NULL,"
            "volume INTEGER NOT NULL,"
            "FOREIGN KEY (security_id) REFERENCES securities (security_id)"
            " ON DELETE CASCADE ON UPDATE NO ACTION,"
            "UNIQUE(security_id, date) ON CONFLICT REPLACE"
            ");";
}

const int Migration_003::id() const
{
    return 3;
}

const QString Migration_003::sql() const
{
    return "CREATE TABLE relations ("
            "security_id1  INTEGER NOT NULL,"
            "security_id2  INTEGER NOT NULL,"
            "correlation REAL,"
            "beta REAL,"
            "FOREIGN KEY (security_id1) REFERENCES securities (security_id)"
            " ON DELETE CASCADE ON UPDATE NO ACTION,"
            "FOREIGN KEY (security_id2) REFERENCES securities (security_id)"
            " ON DELETE CASCADE ON UPDATE NO ACTION,"
            "UNIQUE(security_id1, security_id2) ON CONFLICT REPLACE"
            ");";
}

}
