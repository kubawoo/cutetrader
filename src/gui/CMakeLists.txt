file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB PROJECT_HEADERS "*.h")
file(GLOB QML_FILES "qml/*.qml")
file(GLOB QRC_FILES "*.qrc")

# Set resource aliases for QML files before adding module
foreach(qml_file ${QML_FILES})
    get_filename_component(qml_name ${qml_file} NAME)
    set_source_files_properties(${qml_file} PROPERTIES QT_RESOURCE_ALIAS "qml/${qml_name}")
endforeach()

qt_add_executable(cutetrader-gui
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${QRC_FILES}
)

target_link_libraries(cutetrader-gui PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Qml
    Qt6::Sql
    Qt6::Network
    account
    common
    data
    mocked
    quant
    twsqapi
)

qt_add_qml_module(cutetrader-gui
    URI "cutetrader"
    VERSION 1.0
    QML_FILES ${QML_FILES}
    RESOURCE_PREFIX "/"
)

qt_finalize_executable(cutetrader-gui)