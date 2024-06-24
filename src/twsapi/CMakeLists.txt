file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(twsapi STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(twsapi PRIVATE bidgcc000)
target_include_directories(twsapi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
