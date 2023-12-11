
set(LIBMDML_SOURCES
	libmdml.cpp
	exception.cpp
	debuginfo.cpp
	Application.cpp
	CgiApplication.cpp
	MarkdownRouteHandler.cpp
	CgiRequestClient.cpp
	#CgiRequestHandler.cpp
)

set(LIBMDML_LINK_LIBS ${LIBMDML_LINK_LIBS} ${STACKTRACE_DEP_LIBS} )

if (USE_SYSTEM_CMARK)
	set(LIBMDML_LINK_LIBS ${LIBMDML_LINK_LIBS} ${cmark_LIBRARIES} Threads::Threads )
endif()

list(REMOVE_DUPLICATES LIBMDML_LINK_LIBS)


add_library(mdml SHARED ${LIBMDML_SOURCES})
add_library(mdml_static STATIC ${LIBMDML_SOURCES})
add_library(mdml_test STATIC ${LIBMDML_SOURCES})

target_compile_definitions(mdml_test PUBLIC -DTEST_SOCKET_PATH="/tmp/cgi-req-test.sock")

target_compile_options(mdml PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_options(mdml_static PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_options(mdml_test PRIVATE ${CMAKE_CXX_FLAGS})

set_target_properties(mdml_static PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be static (?)
)
set_target_properties(mdml_test PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be static (?)
)
target_compile_definitions(mdml_test PRIVATE -DTESTING)

# Check if the system is Linux
if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Add the -rdynamic linker option
    target_link_options(mdml PUBLIC -rdynamic)
    target_link_options(mdml_static PUBLIC -rdynamic)
    target_link_options(mdml_test PUBLIC -rdynamic)
endif()

target_link_libraries(mdml PRIVATE ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_static PRIVATE ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_test PRIVATE ${LIBMDML_LINK_LIBS})
