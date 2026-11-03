#include "common/utils.h"

namespace common {



Utils::Utils()
{}

SecurityType Utils::securityTypeFromString(const QString &str)
{
    QString s = str.trimmed().toUpper();

    if(s == "STK" || s == "STOCK") {
        return SecurityType::STOCK;
    }

    if(s == "OPT" || s == "OPTION") {
        return SecurityType::OPTION;
    }

    if(s == "FOP" || s == "FUTURE OPTION") {
        return SecurityType::FUTURE_OPTION;
    }

    if(s == "FUT" || s == "FUTURE") {
        return SecurityType::FUTURE;
    }

    if(s == "IND" || s == "INDEX") {
        return SecurityType::INDEX;
    }

    if(s == "CFD") {
        return SecurityType::CFD;
    }

    if(s == "BAG") {
        return SecurityType::BAG;
    }

    if(s == "BOND") {
        return SecurityType::BOND;
    }

    if(s == "CASH") {
        return SecurityType::CASH;
    }

    if(s == "CMDTY" || s == "COMMODITY") {
        return SecurityType::CMDTY;
    }

    if(s == "CONTFUT" || s == "CONTINUOUS FUTURE") {
        return SecurityType::CONTFUT;
    }

    if(s == "CRYPTO") {
        return SecurityType::CRYPTO;
    }

    if(s == "FUND") {
        return SecurityType::FUND;
    }

    if(s == "IOPT" || s == "INDEX OPTION") {
        return SecurityType::IOPT;
    }

    if(s == "NEWS") {
        return SecurityType::NEWS;
    }

    if(s == "WAR" || s == "WARRANT") {
        return SecurityType::WAR;
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
    case SecurityType::CFD:
        return "cfd";
    case SecurityType::BAG:
        return "bag";
    case SecurityType::BOND:
        return "bond";
    case SecurityType::CASH:
        return "cash";
    case SecurityType::CMDTY:
        return "cmdty";
    case SecurityType::CONTFUT:
        return "contfut";
    case SecurityType::CRYPTO:
        return "crypto";
    case SecurityType::FUND:
        return "fund";
    case SecurityType::IOPT:
        return "iopt";
    case SecurityType::NEWS:
        return "news";
    case SecurityType::WAR:
        return "war";
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
