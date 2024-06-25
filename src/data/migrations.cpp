#include "migrations.h"


const int Migration_001::order() const
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

const int Migration_002::order() const
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
