file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(twsqapi_tests STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(twsqapi_tests PRIVATE
    Qt6::Core
    Qt6::Test
    Qt6::Network
    common
    twsqapi
)

target_include_directories(twsqapi_tests INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

