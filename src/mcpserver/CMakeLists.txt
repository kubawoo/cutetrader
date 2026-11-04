cmake_minimum_required(VERSION 3.16)

# QtMcp components already found in parent CMakeLists.txt
# This directory is only added when QT_MCP_FOUND is TRUE

file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB PROJECT_HEADERS "*.h")

qt_add_executable(cutetrader-mcpserver
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_link_libraries(cutetrader-mcpserver PRIVATE
    Qt6::Core
    Qt6::Network
    Qt6::Sql
    Qt6::McpCommon
    Qt6::McpClient
    Qt6::McpServer
    common
    twsqapi
    mocked
)

qt_finalize_executable(cutetrader-mcpserver)