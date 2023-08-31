


add_library(mdml STATIC libmdml.cpp)  # Replace with your source files
target_link_libraries(mdml PUBLIC maddy)

add_executable(../mdml-cgi mdml.cpp)    # Replace with your source files
target_link_libraries(../mdml-cgi PRIVATE mdml)
target_link_libraries(../mdml-cgi PRIVATE maddy)
