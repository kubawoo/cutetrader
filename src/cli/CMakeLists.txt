file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB PROJECT_HEADERS "*.h")


qt_add_executable(cutetrader-cli
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_link_libraries(cutetrader-cli PRIVATE
    Qt6::Core
    Qt6::Sql
    Qt6::Network
    account
    common
    data
    mocked
    quant
    twsqapi
)

qt_finalize_executable(cutetrader-cli)
