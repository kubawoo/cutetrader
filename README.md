# cutetrader

Qt6/C++23 CLI trading application for Interactive Brokers TWS API.

## Requirements

- C++23 compiler (gcc 12+)
- Qt6 (Core, Sql, Network, Test)
- CMake 3.16+
- A running **IBKR Gateway** or **TWS** instance (default port 4002)

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Usage

A running IBKR Gateway or TWS with API enabled is required on the target host.

```sh
./build/src/cli/cutetrader-cli [options]
```

| Option | Default | Description |
|--------|---------|-------------|
| `--mocked` | `off` | Use mocked IBKR API (no TWS connection needed) |
| `--database <file>` | `cutetrader.db` | SQLite database file (`:memory:` when `--mocked`) |
| `--host <host>` | `localhost` | IBKR Gateway/TWS host |
| `--port <port>` | `4002` | IBKR Gateway/TWS port |
| `--client-id <id>` | `1` | Client ID for TWS API |
| `--account-id <id>` | — | IBKR account id. Leave blank if you only have one account |
| `--debug` | `off` | Enable debug logging |

Pass `--mocked` for offline testing or development without a Gateway connection.

## Tests

```sh
# all tests
./build/test/testrunner/testrunner

# single test case
./build/test/testrunner/testrunner -case DataManagerTest

# list available test cases
./build/test/testrunner/testrunner -functions
```

## Project structure

| Directory | Contents |
|-----------|----------|
| `src/cli/` | CLI executable entrypoint |
| `src/common/` | Interfaces, DTOs (`ITwsClient`, `IAppFactory`), utilities |
| `src/data/` | SQLite database layer, migrations (`DbBuilder`) |
| `src/account/` | Portfolio, account info |
| `src/twsqapi/` | TWS API socket client, message parsing, cache |
| `src/quant/` | Black-Scholes, calculations |
| `src/mocked/` | Mock implementations for testing |
| `test/` | Qt Test unit tests per library |
