file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(quant_tests STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(quant_tests PRIVATE Qt6::Core Qt6::Test quant)
target_include_directories(quant_tests INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

