# cutetrader

Qt6/C++23 CLI trading application for Interactive Brokers TWS API.

## Requirements

- C++23 compiler (gcc 12+)
- Qt6 (Core, Sql, Network, Test)
- CMake 3.16+
- A running **IBKR Gateway** or **TWS** instance (default port 4002)

### Optional: MCP Server

- QtMcp (Qt Model Context Protocol library) — enables `cutetrader-mcpserver`
  - If QtMcp is not found, the MCP server is skipped automatically

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

If QtMcp is installed, CMake will detect it and build `cutetrader-mcpserver`. Otherwise you'll see:
```
-- QtMcp components not found - mcpserver will NOT be built
```

## Usage

A running IBKR Gateway or TWS with API enabled is required on the target host.

```sh
./build/src/cli/cutetrader-cli [options]
```

### MCP Server (optional)

If built, the MCP server provides AI tool integration via Model Context Protocol:

```sh
./build/src/mcpserver/cutetrader-mcpserver [options]
```

| Option | Default | Description |
|--------|---------|-------------|
| `--mocked` | `off` | Use mocked IBKR API (no TWS connection needed) |
| `--backend <type>` | `streamablehttp` | Backend: `stdio`, `sse`, or `streamablehttp` |
| `--address <host:port>` | `127.0.0.1:8000` | Listen address for HTTP backends |
| `--host <host>` | `localhost` | IBKR Gateway/TWS host |
| `--port <port>` | `4002` | IBKR Gateway/TWS port |
| `--client-id <id>` | `1` | Client ID for TWS API |

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
| `src/mcpserver/` | MCP server for AI tool integration (optional, requires QtMcp) |
| `test/` | Qt Test unit tests per library |
