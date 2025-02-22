#include "common/idbmigration.h"
#include <QCryptographicHash>

namespace common {

IDbMigration::IDbMigration()
{}

IDbMigration::~IDbMigration()
{}

QString IDbMigration::hash() const
{
    QString data = sql().join("\n");
    QByteArray hex = QCryptographicHash::hash(data.toUtf8(), QCryptographicHash::Sha256).toHex();
    QString sha256hash(hex);
    return sha256hash;
}

}
