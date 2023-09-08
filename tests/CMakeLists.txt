# CMakeLists.txt in the tests directory

# Add your test executable
add_executable(mdml_tests exceptions-test.cpp)

# Link the test executable with your source code and Catch2
target_link_libraries(mdml_tests PRIVATE mdml Catch2::Catch2WithMain)

# Add the include directories detected by find_package(Catch2)
target_include_directories(mdml_tests PRIVATE ${Catch2_INCLUDE_DIRS})

add_test(NAME mdml_tests COMMAND check)
