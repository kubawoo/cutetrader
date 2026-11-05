cmake_minimum_required(VERSION 3.16)

project(cutetrader VERSION 0.1 LANGUAGES CXX)

# Get git commit hash for version info
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    # Check for uncommitted changes
    execute_process(
        COMMAND ${GIT_EXECUTABLE} status --porcelain
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_STATUS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
    if(GIT_STATUS)
        set(GIT_COMMIT_HASH "${GIT_COMMIT_HASH}-dirty")
    endif()
else()
    set(GIT_COMMIT_HASH "unknown")
endif()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test Sql Network Quick Qml)
qt_standard_project_setup()

# Try to find QtMcp components (optional)
find_package(Qt6 COMPONENTS McpCommon McpClient McpServer QUIET)
set(QT_MCP_FOUND FALSE)
if(Qt6McpCommon_FOUND AND Qt6McpClient_FOUND AND Qt6McpServer_FOUND)
    set(QT_MCP_FOUND TRUE)
    message(STATUS "QtMcp components found - mcpserver will be built")
else()
    message(STATUS "QtMcp components not found - mcpserver will NOT be built")
endif()

add_subdirectory(src/account)
add_subdirectory(src/cli)
add_subdirectory(src/common)
add_subdirectory(src/data)
add_subdirectory(src/gui)
add_subdirectory(src/mocked)
add_subdirectory(src/quant)
add_subdirectory(src/twsqapi)

if(QT_MCP_FOUND)
    add_subdirectory(src/mcpserver)
endif()

add_subdirectory(test/testrunner)

add_subdirectory(test/data)
add_subdirectory(test/quant)
add_subdirectory(test/twsqapi)
