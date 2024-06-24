file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(twsclient STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(twsclient PRIVATE Qt6::Core twsapi)
target_include_directories(twsclient INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
