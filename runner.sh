#!/usr/bin/env bash
# runner.sh - Convenience script to run cutetrader CLI and tests
# Usage: ./runner.sh [options] [command]

set -euo pipefail

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Defaults
BUILD_DIR="build"
CLI_EXE="${BUILD_DIR}/src/cli/cutetrader-cli"
TEST_EXE="${BUILD_DIR}/test/testrunner/testrunner"
MOCKED=""
VALGRIND=""
VALGRIND_OPTS=""
TEST_FILTER=""
VERBOSE=false
COMMAND=""
REBUILD=""

# Help function
show_help() {
    cat << EOF
Usage: ./runner.sh [OPTIONS] [COMMAND]

Commands:
  cli                 Run the CLI application (default)
  test                Run all tests
  test <name>         Run specific test case (e.g., test CacheTest)
  build               Build the project

Options:
  -m, --mocked        Run in mocked mode (no TWS connection)
  -r, --real          Run in real mode (connect to TWS) [default]
  -v, --valgrind      Run under valgrind (no default options)
  -V, --valgrind-opts "OPTS"  Custom valgrind options (default: empty)
  -f, --filter NAME   Filter tests by name (for test command)
  -b, --build-dir DIR Build directory [default: build]
  -h, --help          Show this help
  --verbose           Verbose test output (-v2)

Examples:
  ./runner.sh cli                    # Run CLI in real mode (client-id=0)
  ./runner.sh cli --mocked           # Run CLI in mocked mode (account-id=mocked1)
  ./runner.sh cli --mocked --valgrind # Run CLI mocked under valgrind
  ./runner.sh test                   # Run all tests
  ./runner.sh test CacheTest         # Run only CacheTest
  ./runner.sh test --valgrind        # Run all tests under valgrind
  ./runner.sh build                  # Build project
  ./runner.sh build --rebuild        # Clean build (remove build dir first)
  ./runner.sh build --valgrind       # Build and run tests under valgrind

Defaults:
  Mocked mode:  --account-id mocked1
  Real mode:    --client-id 0

Environment variables:
  HOST, PORT, CLIENT_ID, ACCOUNT_ID, DATABASE - Passed to CLI
EOF
}

# Parse arguments
POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--mocked)
            MOCKED=1
            shift
            ;;
        -r|--real)
            MOCKED=""
            shift
            ;;
        -v|--valgrind)
            VALGRIND=1
            shift
            ;;
        -V|--valgrind-opts)
            VALGRIND_OPTS="$2"
            shift 2
            ;;
        -f|--filter)
            TEST_FILTER="$2"
            shift 2
            ;;
        -b|--build-dir)
            BUILD_DIR="$2"
            CLI_EXE="${BUILD_DIR}/src/cli/cutetrader-cli"
            TEST_EXE="${BUILD_DIR}/test/testrunner/testrunner"
            shift 2
            ;;
        --rebuild)
            REBUILD=1
            shift
            ;;
        --verbose)
            VERBOSE=1
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        cli|test|build)
            COMMAND="$1"
            shift
            ;;
        *)
            POSITIONAL_ARGS+=("$1")
            shift
            ;;
    esac
done

# Handle positional test filter
if [[ ${#POSITIONAL_ARGS[@]} -gt 0 && "$COMMAND" == "test" ]]; then
    TEST_FILTER="${POSITIONAL_ARGS[0]}"
fi

# Show help if no command provided
if [[ -z "$COMMAND" ]]; then
    show_help
    exit 0
fi

# Check executables exist
check_exe() {
    if [[ ! -f "$1" ]]; then
        echo -e "${RED}Error: Executable not found: $1${NC}"
        echo "Run './runner.sh build' first"
        exit 1
    fi
}

# Build command
cmd_build() {
    if [[ -n "$REBUILD" ]]; then
        echo -e "${YELLOW}Removing build directory: $BUILD_DIR${NC}"
        rm -rf "$BUILD_DIR"
    fi
    echo -e "${BLUE}Building project...${NC}"
    cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
    cmake --build "$BUILD_DIR" -j$(nproc)
    echo -e "${GREEN}Build complete!${NC}"
}

# Run CLI
cmd_cli() {
    check_exe "$CLI_EXE"
    
    local args=()
    if [[ -n "$MOCKED" ]]; then
        args+=(--mocked)
    fi
    
    # Set defaults based on mode
    local account_id="${ACCOUNT_ID:-}"
    local client_id="${CLIENT_ID:-}"
    
    if [[ -n "$MOCKED" ]]; then
        # Mocked mode defaults
        account_id="${account_id:-mocked1}"
    else
        # Real mode defaults
        client_id="${client_id:-0}"
    fi
    
    # Pass through environment variables (with defaults)
    [[ -n "${HOST:-}" ]] && args+=(--host "$HOST")
    [[ -n "${PORT:-}" ]] && args+=(--port "$PORT")
    [[ -n "$client_id" ]] && args+=(--client-id "$client_id")
    [[ -n "$account_id" ]] && args+=(--account-id "$account_id")
    [[ -n "${DATABASE:-}" ]] && args+=(--database "$DATABASE")
    
    if [[ -n "$MOCKED" ]]; then
        echo -e "${BLUE}Running CLI (mocked)${NC}"
    else
        echo -e "${BLUE}Running CLI (real)${NC}"
    fi
    echo -e "${YELLOW}Command: $CLI_EXE ${args[*]}${NC}"
    
    if [[ -n "$VALGRIND" ]]; then
        valgrind $VALGRIND_OPTS "$CLI_EXE" "${args[@]}"
    else
        "$CLI_EXE" "${args[@]}"
    fi
}

# Run tests
cmd_test() {
    check_exe "$TEST_EXE"
    
    local args=()
    if [[ -n "$TEST_FILTER" ]]; then
        args+=(-case "$TEST_FILTER")
    fi
    if [[ "$VERBOSE" == true ]]; then
        args+=(-v2)
    fi
    
    echo -e "${BLUE}Running tests${TEST_FILTER:+ (filter: $TEST_FILTER)}${NC}"
    echo -e "${YELLOW}Command: $TEST_EXE ${args[*]}${NC}"
    
    if [[ -n "$VALGRIND" ]]; then
        valgrind $VALGRIND_OPTS "$TEST_EXE" "${args[@]}"
    else
        "$TEST_EXE" "${args[@]}"
    fi
}

# Main dispatch
case "$COMMAND" in
    build)
        cmd_build
        ;;
    cli)
        cmd_cli
        ;;
    test)
        cmd_test
        ;;
    *)
        echo -e "${RED}Unknown command: $COMMAND${NC}"
        show_help
        exit 1
        ;;
esac
