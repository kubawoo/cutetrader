file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "quant/*.h")

add_library(quant STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(quant PRIVATE Qt6::Core)
target_include_directories(quant INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
