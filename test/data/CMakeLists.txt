file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(data_tests STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(data_tests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Sql
    common
    data)

target_include_directories(data_tests INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

