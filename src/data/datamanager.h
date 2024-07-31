#ifndef DATA_DATAMANAGER_H
#define DATA_DATAMANAGER_H

#include <QSqlDatabase>
#include "model.h"
#include "utils.h"

namespace data {

class DataManager
{
public:
    DataManager();
    DataManager(QSqlDatabase & db);

    bool createSecurity(const Security& security);
    Security getSecurity(int id);
    Security getSecurity(const QString & symbol);
    bool removeSecurity(int id);
    QList<Security> getAllSecurities();

private:
    Security toSecurity(DbResult & data);
    Quote toQuote(DbResult & data);

    template <typename T, typename P>
    bool setValue(T& t, void (T::*setter)(P), QVariant& d, P (QVariant::*getter)(bool *) const) {
        bool ok;
        P value = (d.*getter)(&ok);
        if(ok) {
            (t.*setter)(value);
        }
        return ok;
    }

    template <typename T, typename P>
    bool setValue(T& t, void (T::*setter)(const P&), QVariant &d, P (QVariant::*getter)() const) {
        P value = (d.*getter)();
        (t.*setter)(value);
        return true;
    }

    QSqlDatabase _db;
};

}

#endif // DATA_DATAMANAGER_H
