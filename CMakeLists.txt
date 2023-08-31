cmake_minimum_required(VERSION 3.10)  # Adjust the version as needed
project(mdml VERSION 1.0)

include(FetchContent)
include(CheckIncludeFile)

FetchContent_Declare(
   	maddy
    GIT_REPOSITORY https://github.com/progsource/maddy
    GIT_TAG 1.2.1  # Specify a specific version or commit
)

FetchContent_MakeAvailable(maddy)

CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

add_subdirectory(src)

