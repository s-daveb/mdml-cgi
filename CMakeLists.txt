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
	LANGUAGES C CXX
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


# Instead of calling FetchContent_MakeAvailable, using a more manual way
# to import cmark's build targets.    The official cmark repo builds unit
# tests by default, which we don't need here. It also uses deprecated FindPython
# modules that cause the build to spit out warnings.

# Import the build variables from cmark
FetchContent_GetProperties(cmark)

# cmark's cmake uses very deprecated functions for unit tests, and hence
# cannot be imported with FetchContent_MakeAvailable.  A different process
# has to be used to disable the broken build targets. 
if (NOT cmark_POPULATED)            # if build targets not populated

	# Since these properties are /options/ of cmark, they need to be overriden
	# at the CmakeCache.txt level, hence the CACHE INTERNAL keywords

	# Enable shared lib and static lib build for flexibility
	set(CMARK_SHARED ON CACHE INTERNAL "Turn on shared lib")
	set(CMARK_STATIC ON CACHE INTERNAL "Turn on static lib")

	#  disable cmark unit tests that use deprecated features.
	set(CMARK_TESTS OFF CACHE INTERNAL "Turn off tests")


	FetchContent_Populate(cmark)    # populate remaining build targets
	# Add cmark build targets to this project
	add_subdirectory(${cmark_SOURCE_DIR} ${cmark_BINARY_DIR})
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
