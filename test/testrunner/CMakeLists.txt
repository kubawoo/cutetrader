qt_add_executable(testrunner
    multitests.h
    helper.h
    testrunner.cpp
)

target_link_libraries(testrunner PRIVATE
    Qt6::Core
    Qt6::Test
    data_tests
    quant_tests
)
