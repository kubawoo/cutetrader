set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        connectdialog.cpp connectdialog.h connectdialog.ui
)

qt_add_executable(cutetrader
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(cutetrader PRIVATE
    Qt6::Widgets
    Qt6::Sql
    account
    common
    data
    quant
    twsapi
    twsclient
)

qt_finalize_executable(cutetrader)
