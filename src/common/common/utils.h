#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

#include <QString>
#include "types.h"

namespace common {

class Utils
{
private:
    Utils();

public:
    static SecurityType securityTypeFromString(const QString& str);
    static QString securityTypeToString(SecurityType type);
    static OptionType optionTypeFromString(const QString& str);

};
}

#endif // COMMON_UTILS_H
