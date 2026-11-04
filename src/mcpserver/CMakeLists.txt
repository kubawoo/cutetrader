cmake_minimum_required(VERSION 3.16)

# Find QtMcp from the build directory
list(APPEND CMAKE_PREFIX_PATH "/home/kuba/Projects/qtmcp/build/lib64/cmake")

find_package(Qt6 REQUIRED COMPONENTS Core Network Sql McpCommon McpClient McpServer)

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