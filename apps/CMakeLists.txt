
SET(mdml_cgi_sources main.cpp)

add_executable(mdml-cgi ${mdml_cgi_sources})
target_link_libraries(mdml-cgi PRIVATE mdml)
