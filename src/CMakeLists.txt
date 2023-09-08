add_library(mdml SHARED libmdml.cpp)  # Replace with your source files
target_link_libraries(mdml PUBLIC maddy)
