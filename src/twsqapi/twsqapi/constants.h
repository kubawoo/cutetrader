#ifndef TWSQAPI_CONSTANTS_H
#define TWSQAPI_CONSTANTS_H


namespace twsqapi {

namespace client {
constexpr char PACEAPI_OPT[] = "+PACEAPI";
constexpr int SERVER_MIN_VERSION = 198;
constexpr int SERVER_MAX_VERSION = 198;
constexpr char SERVER_VERSIONS_DELIMITER[] = "..";
constexpr char SERVER_VERSION_PREFIX = 'v';
constexpr char API_HEADER[] = "API";
constexpr int MSG_LENGTH_FIELD_SIZE = 4;
constexpr int HANDSHAKE_FIELDS = 2;

}

namespace requests {
constexpr int REQ_ACCT_DATA = 6;
constexpr int REQ_ACCT_DATA_VERSION = 2;

constexpr int REQ_CONTRACT_DATA = 9;
constexpr int REQ_CONTRACT_DATA_VERSION = 8;

constexpr int REQ_CURRENT_TIME = 49;
constexpr int REQ_CURRENT_TIME_VERSION = 1;

constexpr int START_API = 71;
constexpr int START_API_VERSION = 2;

constexpr int REQ_MATCHING_SYMBOLS = 81;




/*
 * const int REQ_MKT_DATA                  = 1;
const int CANCEL_MKT_DATA               = 2;
const int PLACE_ORDER                   = 3;
const int CANCEL_ORDER                  = 4;
const int REQ_OPEN_ORDERS               = 5;
const int REQ_EXECUTIONS                = 7;
const int REQ_IDS                       = 8;
const int REQ_MKT_DEPTH                 = 10;
const int CANCEL_MKT_DEPTH              = 11;
const int REQ_NEWS_BULLETINS            = 12;
const int CANCEL_NEWS_BULLETINS         = 13;
const int SET_SERVER_LOGLEVEL           = 14;
const int REQ_AUTO_OPEN_ORDERS          = 15;
const int REQ_ALL_OPEN_ORDERS           = 16;
const int REQ_MANAGED_ACCTS             = 17;
const int REQ_FA                        = 18;
const int REPLACE_FA                    = 19;
const int REQ_HISTORICAL_DATA           = 20;
const int EXERCISE_OPTIONS              = 21;
const int REQ_SCANNER_SUBSCRIPTION      = 22;
const int CANCEL_SCANNER_SUBSCRIPTION   = 23;
const int REQ_SCANNER_PARAMETERS        = 24;
const int CANCEL_HISTORICAL_DATA        = 25;
const int REQ_REAL_TIME_BARS            = 50;
const int CANCEL_REAL_TIME_BARS         = 51;
const int REQ_FUNDAMENTAL_DATA          = 52;
const int CANCEL_FUNDAMENTAL_DATA       = 53;
const int REQ_CALC_IMPLIED_VOLAT        = 54;
const int REQ_CALC_OPTION_PRICE         = 55;
const int CANCEL_CALC_IMPLIED_VOLAT     = 56;
const int CANCEL_CALC_OPTION_PRICE      = 57;
const int REQ_GLOBAL_CANCEL             = 58;
const int REQ_MARKET_DATA_TYPE          = 59;
const int REQ_POSITIONS                 = 61;
const int REQ_ACCOUNT_SUMMARY           = 62;
const int CANCEL_ACCOUNT_SUMMARY        = 63;
const int CANCEL_POSITIONS              = 64;
const int VERIFY_REQUEST                = 65;
const int VERIFY_MESSAGE                = 66;
const int QUERY_DISPLAY_GROUPS          = 67;
const int SUBSCRIBE_TO_GROUP_EVENTS     = 68;
const int UPDATE_DISPLAY_GROUP          = 69;
const int UNSUBSCRIBE_FROM_GROUP_EVENTS = 70;
const int VERIFY_AND_AUTH_REQUEST       = 72;
const int VERIFY_AND_AUTH_MESSAGE       = 73;
const int REQ_POSITIONS_MULTI           = 74;
const int CANCEL_POSITIONS_MULTI        = 75;
const int REQ_ACCOUNT_UPDATES_MULTI     = 76;
const int CANCEL_ACCOUNT_UPDATES_MULTI  = 77;
const int REQ_SEC_DEF_OPT_PARAMS        = 78;
const int REQ_SOFT_DOLLAR_TIERS         = 79;
const int REQ_FAMILY_CODES              = 80;
const int REQ_MATCHING_SYMBOLS          = 81;
const int REQ_MKT_DEPTH_EXCHANGES       = 82;
const int REQ_SMART_COMPONENTS          = 83;
const int REQ_NEWS_ARTICLE              = 84;
const int REQ_NEWS_PROVIDERS            = 85;
const int REQ_HISTORICAL_NEWS           = 86;
const int REQ_HEAD_TIMESTAMP            = 87;
const int REQ_HISTOGRAM_DATA            = 88;
const int CANCEL_HISTOGRAM_DATA         = 89;
const int CANCEL_HEAD_TIMESTAMP         = 90;
const int REQ_MARKET_RULE               = 91;
const int REQ_PNL                       = 92;
const int CANCEL_PNL                    = 93;
const int REQ_PNL_SINGLE                = 94;
const int CANCEL_PNL_SINGLE             = 95;
const int REQ_HISTORICAL_TICKS          = 96;
const int REQ_TICK_BY_TICK_DATA         = 97;
const int CANCEL_TICK_BY_TICK_DATA      = 98;
const int REQ_COMPLETED_ORDERS          = 99;
const int REQ_WSH_META_DATA				= 100;
const int CANCEL_WSH_META_DATA			= 101;
const int REQ_WSH_EVENT_DATA			= 102;
const int CANCEL_WSH_EVENT_DATA			= 103;
const int REQ_USER_INFO                 = 104;
const int REQ_CURRENT_TIME_IN_MILLIS    = 105;
*/
}

namespace responses {
constexpr int NO_VERSION = -1;
constexpr int UNKNOWN_FIELDS_COUNT = -1;

constexpr int ERR_MSG = 4;
constexpr int ERR_MSG_FIELDS = 6;

constexpr int ACCT_VALUE = 6;
constexpr int ACCT_VALUE_VERSION = 2;
constexpr int ACCT_VALUE_FIELDS = 6;

constexpr int PORTFOLIO_VALUE = 7;
constexpr int PORTFOLIO_VALUE_VERSION = 8;
constexpr int PORTFOLIO_VALUE_FIELDS = 20;

constexpr int ACCT_UPDATE_TIME = 8;
constexpr int ACCT_UPDATE_TIME_VERSION = 1;
constexpr int ACCT_UPDATE_TIME_FIELDS = 3;

constexpr int NEXT_VALID_ID = 9;
constexpr int NEXT_VALID_ID_VERSION = 1;
constexpr int NEXT_VALID_ID_FIELDS = 3;

constexpr int CONTRACT_DATA = 10;

constexpr int MANAGED_ACCOUNTS = 15;
constexpr int MANAGED_ACCOUNTS_VERSION = 1;
constexpr int MANAGED_ACCOUNTS_FIELDS = 3;

constexpr int CURRENT_TIME = 49;
constexpr int CURRENT_TIME_VERSION = 1;
constexpr int CURRENT_TIME_FIELDS = 3;

constexpr int CONTRACT_DATA_END = 52;
constexpr int CONTRACT_DATA_END_VERSION = 1;
constexpr int CONTRACT_DATA_END_FIELDS = 3;

constexpr int ACCT_DOWNLOAD_END = 54;
constexpr int ACCT_DOWNLOAD_END_VERSION = 1;
constexpr int ACCT_DOWNLOAD_END_FIELDS = 3;

constexpr int SYMBOL_SAMPLES = 79;


/*
const int TICK_PRICE                                = 1;
const int TICK_SIZE                                 = 2;
const int ORDER_STATUS                              = 3;
const int OPEN_ORDER                                = 5;
const int ACCT_UPDATE_TIME                          = 8;
const int EXECUTION_DATA                            = 11;
const int MARKET_DEPTH                              = 12;
const int MARKET_DEPTH_L2                           = 13;
const int NEWS_BULLETINS                            = 14;
const int RECEIVE_FA                                = 16;
const int HISTORICAL_DATA                           = 17;
const int BOND_CONTRACT_DATA                        = 18;
const int SCANNER_PARAMETERS                        = 19;
const int SCANNER_DATA                              = 20;
const int TICK_OPTION_COMPUTATION                   = 21;
const int TICK_GENERIC                              = 45;
const int TICK_STRING                               = 46;
const int TICK_EFP                                  = 47;
const int CURRENT_TIME                              = 49;
const int REAL_TIME_BARS                            = 50;
const int FUNDAMENTAL_DATA                          = 51;
const int OPEN_ORDER_END                            = 53;
const int EXECUTION_DATA_END                        = 55;
const int DELTA_NEUTRAL_VALIDATION                  = 56;
const int TICK_SNAPSHOT_END                         = 57;
const int MARKET_DATA_TYPE                          = 58;
const int COMMISSION_AND_FEES_REPORT                = 59;
const int POSITION_DATA                             = 61;
const int POSITION_END                              = 62;
const int ACCOUNT_SUMMARY                           = 63;
const int ACCOUNT_SUMMARY_END                       = 64;
const int VERIFY_MESSAGE_API                        = 65;
const int VERIFY_COMPLETED                          = 66;
const int DISPLAY_GROUP_LIST                        = 67;
const int DISPLAY_GROUP_UPDATED                     = 68;
const int VERIFY_AND_AUTH_MESSAGE_API               = 69;
const int VERIFY_AND_AUTH_COMPLETED                 = 70;
const int POSITION_MULTI                            = 71;
const int POSITION_MULTI_END                        = 72;
const int ACCOUNT_UPDATE_MULTI                      = 73;
const int ACCOUNT_UPDATE_MULTI_END                  = 74;
const int SECURITY_DEFINITION_OPTION_PARAMETER      = 75;
const int SECURITY_DEFINITION_OPTION_PARAMETER_END  = 76;
const int SOFT_DOLLAR_TIERS                         = 77;
const int FAMILY_CODES                              = 78;
const int MKT_DEPTH_EXCHANGES                       = 80;
const int TICK_REQ_PARAMS                           = 81;
const int SMART_COMPONENTS                          = 82;
const int NEWS_ARTICLE                              = 83;
const int TICK_NEWS                                 = 84;
const int NEWS_PROVIDERS                            = 85;
const int HISTORICAL_NEWS                           = 86;
const int HISTORICAL_NEWS_END                       = 87;
const int HEAD_TIMESTAMP                            = 88;
const int HISTOGRAM_DATA                            = 89;
const int HISTORICAL_DATA_UPDATE                    = 90;
const int REROUTE_MKT_DATA_REQ                      = 91;
const int REROUTE_MKT_DEPTH_REQ                     = 92;
const int MARKET_RULE                               = 93;
const int PNL                                       = 94;
const int PNL_SINGLE                                = 95;
const int HISTORICAL_TICKS                          = 96;
const int HISTORICAL_TICKS_BID_ASK                  = 97;
const int HISTORICAL_TICKS_LAST                     = 98;
const int TICK_BY_TICK                              = 99;
const int ORDER_BOUND                               = 100;
const int COMPLETED_ORDER                           = 101;
const int COMPLETED_ORDERS_END                      = 102;
const int REPLACE_FA_END                            = 103;
const int WSH_META_DATA                             = 104;
const int WSH_EVENT_DATA                            = 105;
const int HISTORICAL_SCHEDULE                       = 106;
const int USER_INFO                                 = 107;
const int HISTORICAL_DATA_END                       = 108;
const int CURRENT_TIME_IN_MILLIS                    = 109;

  */
}


}


#endif // TWSQAPI_CONSTANTS_H
