
# Add your test executable
add_executable(mdml_tests exceptions-test.cpp)

# Add the include directories detected by find_package(Catch2)
target_include_directories(mdml_tests PRIVATE ${Catch2_INCLUDE_DIRS})

# Link the test executable with your source code and Catch2
target_link_libraries(mdml_tests PRIVATE mdml Catch2::Catch2WithMain)

include(CTest)
include(ParseAndAddCatchTests)
ParseAndAddCatchTests(tests)

#add_test(NAME mdml_tests COMMAND check)
