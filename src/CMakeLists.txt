
set(LIBMDML_SOURCES libmdml.cpp)


add_library(mdml SHARED ${LIBMDML_SOURCES})
add_library(mdml_static STATIC ${LIBMDML_SOURCES})

target_link_libraries(mdml PRIVATE cmark_static)
target_link_libraries(mdml_static PRIVATE cmark_static)
