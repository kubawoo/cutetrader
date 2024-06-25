file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(data STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(data PRIVATE Qt6::Core Qt6::Sql)
target_include_directories(data INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
