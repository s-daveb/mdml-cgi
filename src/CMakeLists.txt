
set(LIBMDML_SOURCES libmdml.cpp)


add_library(mdml SHARED ${LIBMDML_SOURCES})  # Replace with your source files
add_library(mdml_static STATIC ${LIBMDML_SOURCES})  # Replace with your source files

target_link_libraries(mdml PRIVATE cmark_static)
target_link_libraries(mdml_static PRIVATE cmark_static)
