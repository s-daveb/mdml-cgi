
# Add the test executable

set(MDML_TEST_SOURCE_LIST
	exception-test.cpp
	Result-test.cpp
	Application-test.cpp
	CgiApplication-test.cpp
	MarkdownRouteHandler-test.cpp
)

add_executable(mdml-tests
	${MDML_TEST_SOURCE_LIST}
)

set_target_properties(mdml-tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/artifacts/tests"
)

target_compile_definitions(mdml-tests PUBLIC -DTESTING)


target_link_directories(mdml-tests PRIVATE ${CATCH2_LIBRARY_DIRS})
target_include_directories(mdml-tests PRIVATE ${Catch2_INCLUDE_DIRS})

# Link the test executable with libmdml_static source code and Catch2
target_link_libraries(mdml-tests
PRIVATE
	mdml_test
	Catch2::Catch2WithMain
)

# Extras: Catch2 Ctest integration. @{
# add catch2's ctest CMake module and register the tests defined by mdml-tests
# using catch_discover_tests
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(mdml-tests)
# @}

# Add a custom ctest target to the build system
add_custom_target(ctest
	COMMAND ctest -C $CONFIGURATION --test-dir . --output-on-failure
)
add_dependencies(ctest
	mdml-tests
	copy_assets
)
# vim: ft=cmake sw=4 ts=4 noet sts=4 foldmethod=marker foldmarker=@{,@} :
