#include "mocked/mockmigrations.h"
#include <QCryptographicHash>

namespace mocked {


const int MockMigration_1001::id() const
{
    return 1001;
}

const QStringList MockMigration_1001::sql() const
{
    return {
             "INSERT INTO securities (symbol, contractId) VALUES (\"SPY\", 1);",
             "INSERT INTO securities (symbol, contractId) VALUES (\"TLT\", 2);",
           };
}
}
