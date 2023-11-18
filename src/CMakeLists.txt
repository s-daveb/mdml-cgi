
set(LIBMDML_SOURCES
	libmdml.cpp
	exception.cpp
	debuginfo.cpp
	Application.cpp
	CgiApplication.cpp
	MarkdownRouteHandler.cpp
)

set(LIBMDML_LINK_LIBS ${STACKTRACE_DEP_LIBS})

add_library(mdml SHARED ${LIBMDML_SOURCES})
add_library(mdml_static STATIC ${LIBMDML_SOURCES})
add_library(mdml_test STATIC ${LIBMDML_SOURCES})

target_compile_options(mdml PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_options(mdml_static PRIVATE ${CMAKE_CXX_FLAGS})
target_compile_options(mdml_test PRIVATE ${CMAKE_CXX_FLAGS})

set_target_properties(mdml_static PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be static (?)
)
set_target_properties(mdml_test PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be test (?)
)
target_compile_definitions(mdml_test PUBLIC -DTESTING)

target_link_libraries(mdml PRIVATE cmark)
target_link_libraries(mdml_static PRIVATE cmark)
target_link_libraries(mdml_test PRIVATE cmark)

target_link_libraries(mdml PUBLIC ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_static PUBLIC ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_test PUBLIC ${LIBMDML_LINK_LIBS})
