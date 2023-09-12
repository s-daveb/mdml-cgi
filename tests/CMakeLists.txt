
# Add the test executable
add_executable(mdml-tests exceptions-test.cpp)

target_include_directories(mdml-tests PRIVATE ${Catch2_INCLUDE_DIRS})

# Link the test executable with libmdml_static source code and Catch2
target_link_libraries(mdml-tests PRIVATE mdml_static Catch2::Catch2WithMain)

# Extras: Catch2 Ctest integration. @{
# add catch2's ctest CMake module and register the tests defined by mdml-tests
# using catch_discover_tests
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(mdml-tests)
# @}

# vim: ft=cmake sw=4 ts=4 noet sts=4 foldmethod=marker foldmarker=@{,@} :
