file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "mocked/*.h")

add_library(mocked STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(mocked PRIVATE Qt6::Core Qt6::Sql common)
target_include_directories(mocked INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
