file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "twsqapi/*.h")

add_library(twsqapi STATIC
    ${SOURCES}
    ${HEADERS}
)

target_link_libraries(twsqapi PRIVATE Qt6::Core Qt6::Network common)
target_include_directories(twsqapi INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
