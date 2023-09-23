
set(LIBMDML_SOURCES libmdml.cpp exception.cpp debuginfo.cpp)
set(LIBMDML_LINK_LIBS ${STACKTRACE_DEP_LIBS})

add_library(mdml SHARED ${LIBMDML_SOURCES})
add_library(mdml_static STATIC ${LIBMDML_SOURCES})

set_target_properties(mdml_static PROPERTIES
    CMAKE_POSITION_INDEPENDENT_CODE OFF  # This forces all linkages to be static (?)
)

target_link_libraries(mdml PRIVATE cmark)

target_link_libraries(mdml PUBLIC ${LIBMDML_LINK_LIBS})
target_link_libraries(mdml_static PUBLIC ${LIBMDML_LINK_LIBS})
