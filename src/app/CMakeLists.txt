file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB PROJECT_HEADERS "*.h")
file(GLOB PROJECT_UIS "*.ui")


qt_add_executable(cutetrader
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UIS}
)

target_link_libraries(cutetrader PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::Network
    account
    common
    data
    mocked
    quant
    twsqapi
)

qt_finalize_executable(cutetrader)
