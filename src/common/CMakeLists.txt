file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "common/*.h")

add_library(common STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(common PRIVATE Qt6::Core)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
