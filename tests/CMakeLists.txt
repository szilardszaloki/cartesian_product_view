add_executable(tests tests.cpp)
target_link_libraries(tests gtest_main)
add_test(NAME cartesian_product_view.tests COMMAND tests)