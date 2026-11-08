# cutetrader

Qt6/C++23 CLI trading application for Interactive Brokers TWS API.

## Build & run

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j4

./build/src/cli/cutetrader-cli [--mocked] [--database <file>] [--host localhost] [--port 4002] [--client-id 1] [--account-id <id>] [--debug]
```

Pass `--mocked` to avoid connecting to TWS (uses `mocked::MockAppFactory`).

### CLI Commands

| Command | Description |
|---------|-------------|
| `account` | Account summary and portfolio overview |
| `orders` | List open orders (cached); use `orders refresh` to fetch from TWS |
| `search <pattern>` | Search for contract symbols |
| `historical <symbol> [params]` | Request historical bars |
| `cancel-historical <reqId>` | Cancel historical data request |
| `watchlist` | Show watchlist |
| `time` | Get server time |
| `help` | Show available commands |
| `quit` | Exit CLI |

### MCP Server (optional)

The MCP server (`cutetrader-mcpserver`) is only built if QtMcp components are found:

```sh
# If QtMcp is available, the server is built automatically
./build/src/mcpserver/cutetrader-mcpserver [--mocked] [--backend stdio|sse|streamablehttp] [--address host:port] [--host localhost] [--port 4002] [--client-id 1]
```

QtMcp is an optional dependency. If not found, cmake will print "QtMcp components not found - mcpserver will NOT be built" and skip it.

## Agent constraints

- **Do not access anything outside the project directory** (`$PWD`) — no reading/writing files, running commands, or accessing resources outside this path.

## Tests

Single runner executable links all test libraries.

```sh
# all tests
./build/test/testrunner/testrunner

# single test case (class name, see TEST_DECLARE in *_tests.h)
./build/test/testrunner/testrunner -case DataManagerTest

# list available test cases
./build/test/testrunner/testrunner -functions
```

Test registration pattern: declare test class in `test/<domain>/test_<subject>.h`, register via `TEST_DECLARE(ClassName)` in `test/<domain>/<domain>_tests.h`. Use `MULTI_TESTS_MAIN` (provides QCoreApplication) in runner. Floating-point comparison helper: `kCompare(x, y, precision)` in `helper.h`.

## Architecture

| Directory | Purpose |
|-----------|---------|
| `src/cli/` | CLI executable (`cutetrader-cli`) — entrypoint `main.cpp` |
| `src/common/` | Interfaces (`IAppFactory`, `ITwsClient`, `IDbMigration`), DTOs (`types.h`), utilities |
| `src/data/` | SQL database layer — `DbBuilder` (migrations), `DataManager`, `Model` |
| `src/account/` | Portfolio, account info, account model, **open orders cache** |
| `src/twsqapi/` | TWS API communication — socket client, message parsing, cache |
| `src/quant/` | Quantitative — Black-Scholes, calculations |
| `src/mocked/` | Mock implementations of `IAppFactory`/`ITwsClient` for testing |
| `src/mcpserver/` | MCP (Model Context Protocol) server for AI tool integration (optional, requires QtMcp) |

Includes use `target_include_directories` so headers look like `#include <common/types.h>` (headers live in `src/<lib>/<lib>/`).

## Open Orders Caching

- On startup, after account selection, open orders are **automatically fetched** and cached in `Account`
- The `orders` command shows cached data instantly (no network round-trip)
- `orders refresh` explicitly re-fetches from TWS, updates cache, then displays
- Incoming `orderStatus` messages automatically update cached order status (fill, cancel, reject)
- Manual refresh only needed after reconnection or suspected message loss

## Key conventions

- **C++23** (`CMAKE_CXX_STANDARD 23`), Qt6 (`find_package(Qt6 REQUIRED COMPONENTS Core Test Sql Network)`)
- Static libraries with `file(GLOB ...)` for sources
- Qt Creator project (`.idea/`, `.qtc/` in build dir)
- DB migrations: implement `IDbMigration`, add via `IAppFactory::extraMigrations()`, run by `DbBuilder`
- `.gitignore` ignores `src/twsapi`, `src/twsclient` (upstream IB API), `tcpflow/`, `*.db`, `build/`
- No CI/CD, no lint/format config
