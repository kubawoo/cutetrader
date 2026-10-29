cmake_minimum_required(VERSION 3.16)

project(cutetrader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Test Sql Network Protobuf Quick Qml)
qt_standard_project_setup()


file(GLOB PROTO_SOURCES "../twsapi_macunix.1049.02/IBJts/source/proto/*.proto")

qt_add_protobuf(ProtoMessages
    GENERATE_PACKAGE_SUBFOLDERS
    PROTO_FILES
        ${PROTO_SOURCES}
)


add_subdirectory(src/account)
add_subdirectory(src/cli)
add_subdirectory(src/common)
add_subdirectory(src/data)
add_subdirectory(src/mocked)
add_subdirectory(src/quant)
add_subdirectory(src/twsqapi)

add_subdirectory(test/testrunner)

add_subdirectory(test/data)
add_subdirectory(test/quant)
add_subdirectory(test/twsqapi)
