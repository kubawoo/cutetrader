file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(account STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(account PRIVATE Qt6::Core common)
target_include_directories(account INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
