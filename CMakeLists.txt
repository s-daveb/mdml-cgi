cmake_minimum_required(VERSION 3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED PROJECT_NAME)
	set(NOT_SUBPROJECT ON)
else()
	set(NOT_SUBPROJECT OFF)
endif()

project(mdml
	VERSION 0.1
	LANGUAGES CXX
	# HOMEPAGE_URL
	DESCRIPTION "A CGI script written in C++ that converts markdown to HTML"
)

find_program(CCACHE_PATH ccache)
if(CCACHE_PATH)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
endif(CCACHE_PATH)


include(FetchContent)
include(CheckIncludeFile)

FetchContent_Declare(
   	cmark
	URL https://github.com/commonmark/cmark/archive/refs/tags/0.30.3.zip
	URL_HASH MD5=e806e6430ea49540f512ab98f32d4e2c
)

FetchContent_Declare(
   	Catch2
	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
)


FetchContent_GetProperties(cmark)
if (NOT cmark_POPULATED)
	if (CMARK_TESTS)
		set(CMARK_TESTS OFF)
	endif()
	set(CMARK_SHARED ON)
	FetchContent_Populate(cmark)
endif()
FetchContent_MakeAvailable(Catch2)


CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(custom_dbg_flags "-O0 -ggdb")

    # Custom flags for Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${custom_dbg_flags}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${custom_dbg_flags}")

elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(custom_rel_flags "-Os")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${custom_rel_flags}")
    set(
		CMAKE_CXX_FLAGS_RELEASE
		"${CMAKE_CXX_FLAGS_RELEASE} ${custom_rel_flags}"
	)
endif()


add_subdirectory(src)
add_subdirectory(apps)

if (NOT_SUBPROJECT)
	add_subdirectory(tests)
endif(NOT_SUBPROJECT)
