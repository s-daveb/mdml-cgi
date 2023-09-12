cmake_minimum_required(VERSION 3.24)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

if (NOT DEFINED PROJECT_NAME)
	set(IS_SUBPROJECT OFF)
else()
	set(IS_SUBPROJECT ON)
endif()

project(mdml
	VERSION 0.1
	LANGUAGES C CXX
	# HOMEPAGE_URL
	DESCRIPTION "A CGI script written in C++ that converts markdown to HTML"
	)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CCACHE_PATH ccache)
if(CCACHE_PATH)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
endif(CCACHE_PATH)


set(CMAKE_EXPORT_COMPILE_COMMANDS On)

include(FetchContent)
include(CheckIncludeFile)

FetchContent_Declare(
   	Catch2
	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
	FIND_PACKAGE_ARGS
	)

FetchContent_Declare(
   	cmark
	URL https://github.com/commonmark/cmark/archive/refs/tags/0.30.3.zip
	URL_HASH MD5=e806e6430ea49540f512ab98f32d4e2c
	)
# @}

FetchContent_MakeAvailable(Catch2)
# Just kidding!
# Instead of calling FetchContent_MakeAvailable, use a more manual way
# to import cmark's build targets.https
# The official cmark repo builds unit ests by default, which we don't need here.
# cmarktests also use a deprecated FindPython  modules that cause the build
# to spit out warnings.

# Step 1. Import the build variables from cmark
FetchContent_GetProperties(cmark)

# Step 2. Set Dependency options @{
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
endif() # end Step 2. @}

if (TARGET Catch2)
set_target_properties(Catch2 PROPERTIES
	CXX_STANDARD 17
)
endif()

if (TARGET Catch2WithMain)
set_target_properties(Catch2WithMain PROPERTIES
	CXX_STANDARD 17
) 
endif()

set_target_properties(cmark PROPERTIES
	C_STANDARD 17
	C_EXTENSIONS ON
	)
set_target_properties(cmark_static PROPERTIES
	C_STANDARD 17
	C_EXTENSIONS ON
	)
# @}

CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/bin)

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

if (NOT IS_SUBPROJECT)
	add_subdirectory(tests)
endif(NOT IS_SUBPROJECT)

# vim: ft=cmake sw=4 ts=4 noet sts=4 foldmethod=marker foldmarker=@{,@} :
