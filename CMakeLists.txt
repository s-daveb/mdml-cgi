cmake_minimum_required(VERSION 3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED PROJECT_NAME)
	enable_testing()
endif()

project(mdml
	VERSION 0.1
	LANGUAGES CXX
	# HOMEPAGE_URL
	DESCRIPTION "A CGI script written in C++ that converts markdown to HTML"
)
include(FetchContent)
include(CheckIncludeFile)

FetchContent_Declare(
   	maddy
	URL https://github.com/progsource/maddy/releases/download/1.3.0/maddy-src.zip
	URL_HASH MD5=a29ddce16c2437ec9fac2441c7daa806
)

FetchContent_Declare(
   	Catch2
	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
)

FetchContent_MakeAvailable(maddy)
FetchContent_MakeAvailable(Catch2)

CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)

add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)

