
set(MDML__CGI_SOURCES main.cpp)

add_executable(mdml-cgi ${MDML__CGI_SOURCES})
target_link_libraries(mdml-cgi PRIVATE mdml_static)
