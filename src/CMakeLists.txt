
set(LIBMDML_SOURCES
	libmdml.cpp
	exception.cpp
	debuginfo.cpp
	Application.cpp
	CgiApplication.cpp
	MarkdownRouteHandler.cpp
)

set(LIBMDML_LINK_LIBS ${STACKTRACE_DEP_LIBS} )

if (USE_SYSTEM_CMARK)
	set(LIBMDML_LINK_LIBS ${LIBMDML_LINK_LIBS} ${cmark_LIBRARIES} )
endif()

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
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be static (?)
)
target_compile_definitions(mdml_test PRIVATE -DTESTING)

if (NOT CMARK_STATIC_LINKAGE)
	target_link_libraries(mdml PRIVATE cmark)
	target_link_libraries(mdml PRIVATE cmark)
	target_link_libraries(mdml_test PRIVATE cmark)
else()
	target_link_libraries(mdml PRIVATE cmark_static)
	target_link_libraries(mdml_static PRIVATE cmark_static)
	target_link_libraries(mdml_test PRIVATE cmark_static)
endif()

target_link_libraries(mdml PRIVATE ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_static PRIVATE ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_test PRIVATE ${LIBMDML_LINK_LIBS})
