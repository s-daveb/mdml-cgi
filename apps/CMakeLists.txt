
set(MDML_CGI_SOURCES mdml.cpp)

add_executable(mdml-cgi ${MDML_CGI_SOURCES})
target_link_libraries(mdml-cgi PRIVATE mdml_static)

add_dependencies(mdml-cgi copy_assets)
