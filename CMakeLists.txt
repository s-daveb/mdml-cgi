cmake_minimum_required(VERSION 3.24)

# Prevent in-source builds @
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same.")
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

# When this package is included as a subproject, there's no need to
# build and run the unit-tests. setting -D ENABLE_TESTS to false could
# speed up build time for pacakge maintainers
if (NOT DEFINED PROJECT_NAME)
	option(ENABLE_TESTS "Build and run unit tests" ON)
else()
	option(ENABLE_TESTS "Build and run unit tests" OFF)
endif()

if (UNIX AND NOT(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux"))
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" OFF)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" ON)
else()
	option(USE_BOOST_STACKTRACE "Use Boost::stacktrace for stack traces" ON)
	option(USE_EXECINFO_STACKTRACE "Use BSD/UNIX execinfo for stack traces" OFF)
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

# @{ Nice-to-haves, ccache and compile_commands.json for clangd
find_program(CCACHE_PATH ccache)
if(CCACHE_PATH)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PATH})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_PATH})
endif(CCACHE_PATH)

set(CMAKE_EXPORT_COMPILE_COMMANDS On)
# @}

include(FetchContent)
include(CheckIncludeFile)

if(ENABLE_TESTS)
FetchContent_Declare(
   	Catch2
	URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.4.0.zip
	URL_HASH MD5=c426e77d4ee0055410bc930182959ae5
	FIND_PACKAGE_ARGS
	)
endif()

FetchContent_Declare(
   	cmark
	URL https://github.com/commonmark/cmark/archive/refs/tags/0.30.3.zip
	URL_HASH MD5=e806e6430ea49540f512ab98f32d4e2c
	)
# @}

# Avoid building Catch2 if unit tests aren't going to be built
if (ENABLE_TESTS)
	FetchContent_MakeAvailable(Catch2)
endif()

# FetchContent: cmark @{
# Instead of calling FetchContent_MakeAvailable, use a more manual way
# to import cmark's build targets
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
# @}

if (TARGET Catch2)
	set_target_properties(Catch2 PROPERTIES CXX_STANDARD 17)
endif()

if (TARGET Catch2WithMain)
	set_target_properties(Catch2WithMain PROPERTIES CXX_STANDARD 17)
endif()

set_target_properties(
	cmark PROPERTIES
	C_STANDARD 17
	C_EXTENSIONS ON
	)
set_target_properties(
	cmark_static PROPERTIES
	C_STANDARD 17
	C_EXTENSIONS ON
	)
# @}

if (USE_EXECINFO_STACKTRACE)
	CHECK_INCLUDE_FILE("execinfo.h" HAVE_EXECINFO_H)

	if (HAVE_EXECINFO_H)
		add_definitions(-DHAVE_EXECINFO_H=1)
	endif()

	find_library(LIB_EXEC_INFO
    	NAMES execinfo  # Specify the library name without the 'lib' prefix or file extension
    	HINTS /usr/lib /usr/local/lib  # Optional hint for the library location
	)
	if (LIB_EXEC_INFO)
   	   	message(STATUS "Found libexecinfo: ${LIB_EXEC_INFO}")
		set(STACKTRACE_DEP_LIBS ${LIB_EXEC_INFO})
	endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/artifacts/bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(custom_dbg_flags "-O0 -ggdb")

    # Custom flags for Debug configuration
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${custom_dbg_flags}")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${custom_dbg_flags}")

	if(USE_BOOST_STACKTRACE)
		set(Boost_USE_STATIC_LIBS ON)
	endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(custom_rel_flags "-Os")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${custom_rel_flags}")
    set(
		CMAKE_CXX_FLAGS_RELEASE
		"${CMAKE_CXX_FLAGS_RELEASE} ${custom_rel_flags}"
	)

	if(USE_BOOST_STACKTRACE)
		set(Boost_USE_STATIC_LIBS OFF)
	endif()
endif()

if(USE_BOOST_STACKTRACE)
	set(Boost_USE_STATIC_RUNTIME OFF)  # Do not require static C++ runtime
	set(Boost_USE_MULTITHREADED ON)
	find_package(Boost 1.82.0 COMPONENTS system filesystem REQUIRED)
endif()

if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	set(STACKTRACE_DEP_LIBS  ${Boost_LIBRARIES})
endif()

add_subdirectory(src)
add_subdirectory(apps)

if (ENABLE_TESTS)
	add_subdirectory(tests)
endif()

# vim: ft=cmake sw=4 ts=4 noet sts=4 foldmethod=marker foldmarker=@{,@} :
