set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui

)

qt_add_executable(cutetrader
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(cutetrader PRIVATE Qt6::Widgets twsapi quant twsclient)

qt_finalize_executable(cutetrader)
