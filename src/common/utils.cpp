#include "common/utils.h"

namespace common {



Utils::Utils()
{}

SecurityType Utils::securityTypeFromString(const QString &str)
{
    if(str == "STK" || str == "stock") {
        return SecurityType::STOCK;
    }

    if(str == "OPT" || str == "option") {
        return SecurityType::OPTION;
    }

    if(str == "FOP" || str == "future option") {
        return SecurityType::FUTURE_OPTION;
    }

    if(str == "FUT" || str == "future") {
        return SecurityType::FUTURE;
    }

    if(str == "IND" || str == "index") {
        return SecurityType::INDEX;
    }

    return SecurityType::UNSUPPORTED;

}

QString Utils::securityTypeToString(SecurityType type)
{
    switch(type) {
    case SecurityType::STOCK:
        return "stock";
    case SecurityType::OPTION:
        return "option";
    case SecurityType::FUTURE:
        return "future";
    case SecurityType::FUTURE_OPTION:
        return "future option";
    case SecurityType::INDEX:
        return "index";
    default:
        return "";
    }
    return "";
}

OptionType Utils::optionTypeFromString(const QString &str)
{
    return str.toLower() == "c" ? OptionType::CALL : OptionType::PUT;
}


}
