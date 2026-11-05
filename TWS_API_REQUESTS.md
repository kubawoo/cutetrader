# TWS API Requests - Implementation Status

This document maps all Interactive Brokers TWS API requests (from the original `_twsapi/source/cppclient/client/EClient.h`) to their implementation status in the custom `src/twsqapi/` codebase.

## Legend

| Status | Meaning |
|--------|---------|
| ✅ **Implemented** | Fully implemented with request message, response handling, and signal emission |
| 🔶 **Partial** | Method exists in interface/implementation but is empty or incomplete |
| ❌ **Not Implemented** | No implementation in custom codebase |
| 📝 **Stub Only** | Only declared in `ITwsClient` interface |

---

## Request Summary Table

| Msg ID | Original Request | Custom Implementation | Status | Notes |
|--------|------------------|----------------------|--------|-------|
| 1 | `reqMktData` | `requestMktData()` / `RequestMktDataClientMessage` | ✅ | Market data subscription with streaming & snapshot |
| 2 | `cancelMktData` | `cancelMktData()` / `CancelMktDataClientMessage` | ✅ | Cancel market data subscription |
| 3 | `placeOrder` | — | ❌ | Order placement |
| 4 | `cancelOrder` | — | ❌ | Order cancellation |
| 5 | `reqOpenOrders` | `requestOpenOrders()` | 🔶 | Stub in `ITwsClient` & `TwsClient` |
| 6 | `reqAccountUpdates` | `startClient()` / `StartAccountUpdatesClientMessage` | ✅ | Account updates subscription |
| 7 | `reqExecutions` | — | ❌ | Execution reports |
| 8 | `reqIds` | — | ❌ | Request next valid order IDs |
| 9 | `reqContractDetails` | `requestContractDetails()` / `RequestContractDetailsClientMessage` | ✅ | Contract details with caching |
| 10 | `reqMktDepth` | — | ❌ | Market depth (Level 2) |
| 11 | `cancelMktDepth` | — | ❌ | Cancel market depth |
| 12 | `reqNewsBulletins` | — | ❌ | News bulletins subscription |
| 13 | `cancelNewsBulletins` | — | ❌ | Cancel news bulletins |
| 14 | `setServerLogLevel` | — | ❌ | Set server log level |
| 15 | `reqAutoOpenOrders` | — | ❌ | Auto bind open orders |
| 16 | `reqAllOpenOrders` | — | ❌ | All open orders |
| 17 | `reqManagedAccts` | `requestManagedAccounts()` | 🔶 | Stub - empty implementation |
| 18 | `requestFA` | — | ❌ | Financial Advisor data |
| 19 | `replaceFA` | — | ❌ | Replace FA data |
| 20 | `reqHistoricalData` | `requestHistoricalData()` / `RequestHistoricalDataClientMessage` | ✅ | Historical bars |
| 21 | `exerciseOptions` | — | ❌ | Exercise options |
| 22 | `reqScannerSubscription` | — | ❌ | Scanner subscription |
| 23 | `cancelScannerSubscription` | — | ❌ | Cancel scanner |
| 24 | `reqScannerParameters` | — | ❌ | Scanner parameters |
| 25 | `cancelHistoricalData` | `cancelHistoricalData()` / `CancelHistoricalDataClientMessage` | ✅ | Cancel historical data request |
| 49 | `reqCurrentTime` | `requestCurrentTime()` / `RequestCurrentTimeClientMessage` | ✅ | Current server time |
| 50 | `reqRealTimeBars` | — | ❌ | Real-time bars (5-sec) |
| 51 | `cancelRealTimeBars` | — | ❌ | Cancel real-time bars |
| 54 | `calculateImpliedVolatility` | — | ❌ | Calculate IV |
| 55 | `calculateOptionPrice` | — | ❌ | Calculate option price |
| 56 | `cancelCalculateImpliedVolat` | — | ❌ | Cancel IV calculation |
| 57 | `cancelCalculateOptionPrice` | — | ❌ | Cancel option price calc |
| 58 | `reqGlobalCancel` | — | ❌ | Global order cancel |
| 59 | `reqMarketDataType` | — | ❌ | Market data type (frozen/delayed) |
| 61 | `reqPositions` | — | ❌ | Position updates |
| 62 | `reqAccountSummary` | — | ❌ | Account summary |
| 63 | `cancelAccountSummary` | — | ❌ | Cancel account summary |
| 64 | `cancelPositions` | — | ❌ | Cancel position updates |
| 65 | `verifyRequest` | — | ❌ | API verification |
| 66 | `verifyMessage` | — | ❌ | Verify message |
| 67 | `queryDisplayGroups` | — | ❌ | Display groups |
| 68 | `subscribeToGroupEvents` | — | ❌ | Group events |
| 69 | `updateDisplayGroup` | — | ❌ | Update display group |
| 70 | `unsubscribeFromGroupEvents` | — | ❌ | Unsubscribe group events |
| 71 | `startApi` | Handshake + `StartApiClientMessage` | ✅ | Implicit via connection handshake |
| 72 | `verifyAndAuthRequest` | — | ❌ | Verify & auth |
| 73 | `verifyAndAuthMessage` | — | ❌ | Verify & auth message |
| 74 | `reqPositionsMulti` | — | ❌ | Multi-account positions |
| 75 | `cancelPositionsMulti` | — | ❌ | Cancel multi positions |
| 76 | `reqAccountUpdatesMulti` | — | ❌ | Multi-account updates |
| 77 | `cancelAccountUpdatesMulti` | — | ❌ | Cancel multi updates |
| 78 | `reqSecDefOptParams` | — | ❌ | Option chain parameters |
| 79 | `reqSoftDollarTiers` | — | ❌ | Soft dollar tiers |
| 80 | `reqFamilyCodes` | — | ❌ | Family codes |
| 81 | `reqMatchingSymbols` | `requestMatchingSymbols()` / `RequestMatchingSymbolsClientMessage` | ✅ | Symbol search with caching |
| 82 | `reqMktDepthExchanges` | — | ❌ | Market depth exchanges |
| 83 | `reqSmartComponents` | — | ❌ | Smart routing components |
| 84 | `reqNewsArticle` | — | ❌ | News article |
| 85 | `reqNewsProviders` | — | ❌ | News providers |
| 86 | `reqHistoricalNews` | — | ❌ | Historical news |
| 87 | `reqHeadTimestamp` | — | ❌ | Head timestamp |
| 88 | `reqHistogramData` | — | ❌ | Histogram data |
| 89 | `cancelHistogramData` | — | ❌ | Cancel histogram |
| 90 | `cancelHeadTimestamp` | — | ❌ | Cancel head timestamp |
| 91 | `reqMarketRule` | — | ❌ | Market rule |
| 92 | `reqPnL` | — | ❌ | PnL updates |
| 93 | `cancelPnL` | — | ❌ | Cancel PnL |
| 94 | `reqPnLSingle` | — | ❌ | Single contract PnL |
| 95 | `cancelPnLSingle` | — | ❌ | Cancel single PnL |
| 96 | `reqHistoricalTicks` | — | ❌ | Historical ticks |
| 97 | `reqTickByTickData` | — | ❌ | Tick-by-tick data |
| 98 | `cancelTickByTickData` | — | ❌ | Cancel tick-by-tick |
| 99 | `reqCompletedOrders` | — | ❌ | Completed orders |
| 100 | `reqWshMetaData` | — | ❌ | WSH metadata |
| 101 | `cancelWshMetaData` | — | ❌ | Cancel WSH metadata |
| 102 | `reqWshEventData` | — | ❌ | WSH event data |
| 103 | `cancelWshEventData` | — | ❌ | Cancel WSH event data |
| 104 | `reqUserInfo` | — | ❌ | User info |
| 105 | `reqCurrentTimeInMillis` | — | ❌ | Current time (millis) |
| 106 | `cancelContractData` | — | ❌ | Cancel contract details |
| 107 | `cancelHistoricalTicks` | — | ❌ | Cancel historical ticks |
| 108 | `reqConfig` | — | ❌ | Request config |
| 109 | `updateConfig` | — | ❌ | Update config |

---

## Detailed Implementation Status

### ✅ Fully Implemented Requests

#### 1. **START_API (71)** - API Handshake
- **Custom**: Handled implicitly during connection via `SocketClient::_connectHandshake()`
- **Message**: `HandshakeClientMessage` (sends `+PACEAPI`)
- **Response**: Server version, connection time

#### 9. **CANCEL_HISTORICAL_DATA (25)** - Cancel Historical Data
- **Custom**: `TwsClient::cancelHistoricalData(reqId)` → `CancelHistoricalDataClientMessage`
- **Response**: Server stops sending historical data for the request ID
- **CLI**: `cancel-historical <reqId>`

#### 2. **REQ_ACCT_DATA (6)** - Account Updates
- **Custom**: Handled implicitly during connection via `SocketClient::_connectHandshake()`
- **Message**: `HandshakeClientMessage` (sends `+PACEAPI`)
- **Response**: Server version, connection time

#### 2. **REQ_ACCT_DATA (6)** - Account Updates
- **Custom**: `TwsClient::startClient(accountId)` → `StartAccountUpdatesClientMessage`
- **Response Handling**:
  - `MANAGED_ACCOUNTS` → `managedAccountsSignal(accounts)`
  - `ACCT_VALUE` → `accountValueUpdatedSignal(key, value, currency)`
  - `ACCT_UPDATE_TIME` → `updateAccountTimeSignal(time)`
  - `ACCT_DOWNLOAD_END` → (handled silently)
  - `PORTFOLIO_VALUE` → `portfolioPositionUpdatedSignal(position)`

#### 3. **REQ_CURRENT_TIME (49)** - Current Server Time
- **Custom**: `TwsClient::requestCurrentTime()` → `RequestCurrentTimeClientMessage`
- **Response**: `CURRENT_TIME` → `currentTimeSignal(QDateTime)`

#### 4. **REQ_CONTRACT_DATA (9)** - Contract Details
- **Custom**: `TwsClient::requestContractDetails(contractId, reqId)` → `RequestContractDetailsClientMessage`
- **Features**: In-memory caching via `Cache` + `ContractDetailsCacheEntry`
- **Response Handling**:
  - `CONTRACT_DATA` → accumulates `ContractDetailsDTO`
  - `CONTRACT_DATA_END` → emits `contractDetailReadySignal(reqId, details)`

#### 5. **REQ_MATCHING_SYMBOLS (81)** - Symbol Search
- **Custom**: `TwsClient::requestMatchingSymbols(pattern, reqId)` → `RequestMatchingSymbolsClientMessage`
- **Features**: In-memory caching via `Cache` + `ContractDetailsCacheEntry`
- **Response Handling**:
  - `SYMBOL_SAMPLES` → accumulates `ContractDetailsDTO`, emits `matchingSymbolsReadySignal(reqId, details)`

---

### 🔶 Partially Implemented / Stubs

#### 6. **REQ_MANAGED_ACCTS (17)** - Managed Accounts
- **Interface**: `ITwsClient::requestManagedAccounts()` (pure virtual)
- **Implementation**: `TwsClient::requestManagedAccounts()` — **empty body**
- **Note**: Managed accounts are actually received automatically after `REQ_ACCT_DATA` via `MANAGED_ACCOUNTS` response

#### 7. **REQ_OPEN_ORDERS (5)** - Open Orders
- **Interface**: `ITwsClient::requestOpenOrders()` (pure virtual)
- **Implementation**: `TwsClient::requestOpenOrders()` — **empty body**
- **Note**: No request message or response handling exists

---

### 📝 Interface Only (Declared in ITwsClient)

These are declared in `common::ITwsClient` but have no implementation in `TwsClient`:
- `requestManagedAccounts()` — empty stub
- `requestOpenOrders()` — empty stub

---

### ❌ Not Implemented (All Remaining Requests)

The following categories have **no implementation** in the custom codebase:

| Category | Requests |
|----------|----------|
| **Market Data** | `reqMktDepth`, `cancelMktDepth`, `reqMarketDataType`, `reqRealTimeBars`, `cancelRealTimeBars`, `reqTickByTickData`, `cancelTickByTickData` |
| **Order Management** | `placeOrder`, `cancelOrder`, `reqOpenOrders`, `reqAllOpenOrders`, `reqAutoOpenOrders`, `reqGlobalCancel`, `reqCompletedOrders` |
| **Executions** | `reqExecutions`, `reqIds` |
| **Historical Data** | `reqHistoricalTicks`, `cancelHistoricalTicks`, `reqHeadTimestamp`, `cancelHeadTimestamp`, `reqHistogramData`, `cancelHistogramData` |
| **Market Depth Exchanges** | `reqMktDepthExchanges`, `reqSmartComponents` |
| **Options Analytics** | `calculateImpliedVolatility`, `calculateOptionPrice`, `cancelCalculateImpliedVolat`, `cancelCalculateOptionPrice`, `reqSecDefOptParams`, `exerciseOptions` |
| **Scanner** | `reqScannerSubscription`, `cancelScannerSubscription`, `reqScannerParameters` |
| **News** | `reqNewsBulletins`, `cancelNewsBulletins`, `reqNewsArticle`, `reqNewsProviders`, `reqHistoricalNews`, `reqWshMetaData`, `cancelWshMetaData`, `reqWshEventData`, `cancelWshEventData` |
| **Account/Positions** | `reqPositions`, `cancelPositions`, `reqAccountSummary`, `cancelAccountSummary`, `reqPositionsMulti`, `cancelPositionsMulti`, `reqAccountUpdatesMulti`, `cancelAccountUpdatesMulti` |
| **PnL** | `reqPnL`, `cancelPnL`, `reqPnLSingle`, `cancelPnLSingle` |
| **Financial Advisor** | `requestFA`, `replaceFA` |
| **Display Groups** | `queryDisplayGroups`, `subscribeToGroupEvents`, `updateDisplayGroup`, `unsubscribeFromGroupEvents` |
| **Authentication** | `verifyRequest`, `verifyMessage`, `verifyAndAuthRequest`, `verifyAndAuthMessage` |
| **Market Rules** | `reqMarketRule` |
| **User Info** | `reqUserInfo` |
| **Soft Dollars / Family Codes** | `reqSoftDollarTiers`, `reqFamilyCodes` |
| **Config** | `reqConfig`, `updateConfig` |
| **Time** | `reqCurrentTimeInMillis` |
| **Contract Data Cancel** | `cancelContractData` |

---

## Response Message Coverage

| Response Msg ID | Response Name | Handled in Custom Code |
|-----------------|---------------|------------------------|
| 4 | `ERR_MSG` | ✅ `ErrorServerMessage` → `_handleErrorMsg` |
| 6 | `ACCT_VALUE` | ✅ `AccountValueServerMessage` → `_handleAccountValue` |
| 7 | `PORTFOLIO_VALUE` | ✅ `PortfolioValueServerMessage` → `_handlePortfolioValue` |
| 8 | `ACCT_UPDATE_TIME` | ✅ `AccountUpdateTimeServerMessage` → `_handleAccountUpdateTime` |
| 9 | `NEXT_VALID_ID` | ✅ `NextValidIdServerMessage` → `_handleNextValidId` |
| 10 | `CONTRACT_DATA` | ✅ `ContractDataServerMessage` → `_handleContractData` |
| 15 | `MANAGED_ACCOUNTS` | ✅ `ManagedAccountsServerMessage` → `_handleManagedAccounts` |
| 49 | `CURRENT_TIME` | ✅ `CurrentTimeServerMessage` → `_handleCurrentTime` |
| 52 | `CONTRACT_DATA_END` | ✅ `ContractDataEndServerMessage` → `_handleContractDataEnd` |
| 54 | `ACCT_DOWNLOAD_END` | ✅ (silent) |
| 79 | `SYMBOL_SAMPLES` | ✅ `SymbolSamplesServerMessage` → `_handleSymbolSamples` |
| 1 | `TICK_PRICE` | ✅ `TickPriceServerMessage` → `_handleTickPrice` |
| 2 | `TICK_SIZE` | ✅ `TickSizeServerMessage` → `_handleTickSize` |
| 45 | `TICK_GENERIC` | ✅ `TickGenericServerMessage` → `_handleTickGeneric` |
| 46 | `TICK_STRING` | ✅ `TickStringServerMessage` → `_handleTickString` |
| 21 | `TICK_OPTION_COMPUTATION` | ✅ `TickOptionComputationServerMessage` → `_handleTickOptionComputation` |
| 57 | `TICK_SNAPSHOT_END` | ✅ `TickSnapshotEndServerMessage` → `_handleTickSnapshotEnd` |
| 58 | `MARKET_DATA_TYPE` | ✅ `MarketDataTypeServerMessage` → `_handleMarketDataType` |

**Not handled**: All other response types (order status, executions, historical data, scanner, news, PnL, etc.)

---

## Architecture Notes

### Custom Message Classes (`src/twsqapi/twsqapi/clientmessage.h`)
| Class | TWS Request | Status |
|-------|-------------|--------|
| `HandshakeClientMessage` | START_API (implicit) | ✅ |
| `StartApiClientMessage` | START_API (explicit) | ✅ (unused?) |
| `StartAccountUpdatesClientMessage` | REQ_ACCT_DATA | ✅ |
| `RequestCurrentTimeClientMessage` | REQ_CURRENT_TIME | ✅ |
| `RequestMatchingSymbolsClientMessage` | REQ_MATCHING_SYMBOLS | ✅ |
| `RequestContractDetailsClientMessage` | REQ_CONTRACT_DATA | ✅ |
| `RequestHistoricalDataClientMessage` | REQ_HISTORICAL_DATA | ✅ |
| `RequestMktDataClientMessage` | REQ_MKT_DATA | ✅ |
| `CancelMktDataClientMessage` | CANCEL_MKT_DATA | ✅ |

### Custom Response Classes (`src/twsqapi/twsqapi/servermessage.h`)
| Class | TWS Response | Status |
|-------|--------------|--------|
| `ManagedAccountsServerMessage` | MANAGED_ACCOUNTS | ✅ |
| `NextValidIdServerMessage` | NEXT_VALID_ID | ✅ |
| `ErrorServerMessage` | ERR_MSG | ✅ |
| `AccountValueServerMessage` | ACCT_VALUE | ✅ |
| `AccountUpdateTimeServerMessage` | ACCT_UPDATE_TIME | ✅ |
| `AccountDownloadEndServerMessage` | ACCT_DOWNLOAD_END | ✅ |
| `CurrentTimeServerMessage` | CURRENT_TIME | ✅ |
| `SymbolSamplesServerMessage` | SYMBOL_SAMPLES | ✅ |
| `ContractDataServerMessage` | CONTRACT_DATA | ✅ |
| `ContractDataEndServerMessage` | CONTRACT_DATA_END | ✅ |
| `HistoricalDataServerMessage` | HISTORICAL_DATA | ✅ |
| `HistoricalDataEndServerMessage` | HISTORICAL_DATA_END | ✅ |
| `HistoricalDataUpdateServerMessage` | HISTORICAL_DATA_UPDATE | ✅ |
| `PortfolioValueServerMessage` | PORTFOLIO_VALUE | ✅ |
| `TickPriceServerMessage` | TICK_PRICE | ✅ |
| `TickSizeServerMessage` | TICK_SIZE | ✅ |
| `TickGenericServerMessage` | TICK_GENERIC | ✅ |
| `TickStringServerMessage` | TICK_STRING | ✅ |
| `TickOptionComputationServerMessage` | TICK_OPTION_COMPUTATION | ✅ |
| `TickSnapshotEndServerMessage` | TICK_SNAPSHOT_END | ✅ |
| `MarketDataTypeServerMessage` | MARKET_DATA_TYPE | ✅ |

### Missing Response Classes Needed for Full Implementation
- Market data: `TickEfp`
- Order: `OpenOrder`, `OrderStatus`, `OpenOrderEnd`
- Executions: `ExecutionData`, `ExecutionDataEnd`, `CommissionAndFeesReport`
- Market depth: `MarketDepth`, `MarketDepthL2`
- Historical: `RealTimeBars`
- Scanner: `ScannerParameters`, `ScannerData`
- News: `NewsBulletins`, `NewsArticle`, `TickNews`, `NewsProviders`, `HistoricalNews`, `HistoricalNewsEnd`
- Positions: `PositionData`, `PositionEnd`, `PositionMulti`, `PositionMultiEnd`
- Account summary: `AccountSummary`, `AccountSummaryEnd`, `AccountUpdateMulti`, `AccountUpdateMultiEnd`
- PnL: `PnL`, `PnLSingle`
- Fundamentals: `FundamentalData`
- Display groups: `DisplayGroupList`, `DisplayGroupUpdated`
- Verification: `VerifyMessageApi`, `VerifyCompleted`, `VerifyAndAuthMessageApi`, `VerifyAndAuthCompleted`
- WSH: `WshMetaData`, `WshEventData`
- Market rule: `MarketRule`
- Smart components: `SmartComponents`
- Histogram: `HistogramData`
- Head timestamp: `HeadTimestamp`
- Reroute: `RerouteMktDataReq`, `RerouteMktDepthReq`
- User info: `UserInfo`
- Current time millis: `CurrentTimeInMillis`
- Config: (none defined)

---

## Recommendations for Next Implementation Steps

1. **Priority 1 - Core Trading**:
   - `placeOrder` / `cancelOrder` (order management)
   - `reqOpenOrders` / `reqAllOpenOrders` / `reqCompletedOrders`
   - `reqExecutions`

2. **Priority 2 - Account & Portfolio**:
   - `reqPositions` / `cancelPositions`
   - `reqAccountSummary` / `cancelAccountSummary`
   - `reqPnL` / `cancelPnL`

3. **Priority 3 - Historical Data**:
   - `cancelHistoricalData`
   - `reqRealTimeBars` / `cancelRealTimeBars`

4. **Priority 4 - Market Depth & Options**:
   - `reqMktDepth` / `cancelMktDepth`
   - `reqSecDefOptParams`
   - `calculateImpliedVolatility` / `calculateOptionPrice`

5. **Priority 5 - Scanner & News**:
   - `reqScannerSubscription` / `cancelScannerSubscription`
   - `reqNewsArticle` / `reqNewsProviders` / `reqHistoricalNews`

---

## File References

| File | Purpose |
|------|---------|
| `src/common/common/itwsclient.h` | Interface definition |
| `src/twsqapi/twsqapi/twsclient.h` | TwsClient declaration |
| `src/twsqapi/twsclient.cpp` | TwsClient implementation |
| `src/twsqapi/twsqapi/clientmessage.h` | Outgoing message classes |
| `src/twsqapi/twsqapi/servermessage.h` | Incoming message classes |
| `src/twsqapi/twsqapi/constants.h` | Message ID constants |
| `_twsapi/source/cppclient/client/EClient.h` | Original TWS API reference |

---

*Generated by analyzing `_twsapi/source/cppclient/client/EClient.h` and `src/twsqapi/` implementation.*