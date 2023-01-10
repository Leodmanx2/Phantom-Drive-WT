# Project Metadata
cmake_minimum_required(VERSION 3.12)
project(PhantomEngine)

# C++ Standards
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Cache Variables
set(ENABLE_NONPORTABLE_OPTIMIZATIONS OFF CACHE BOOL "Enables optimizations that may prevent the target from working on other systems")
set(BUILD_TESTS OFF CACHE BOOL "Build tests")
set(BUILD_EXAMPLES OFF CACHE BOOL "Build example applications")

# Compiler Flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-march=native" COMPILER_SUPPORTS_MARCH_NATIVE)
if(COMPILER_SUPPORTS_MARCH_NATIVE AND ENABLE_NONPORTABLE_OPTIMIZATIONS)
		add_compile_options(PRIVATE -march=native>)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(PRIVATE -Wdeprecated;-Wdocumentation;-Werror=documentation)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(PRIVATE
	  -Wall
	  -Wextra
	  -Wpedantic
	  -Wcast-align
	  -Wcast-qual
	  -Wctor-dtor-privacy
	  -Wdisabled-optimization
	  -Wformat=2
	  -Winit-self
	  -Wmissing-declarations
	  -Wmissing-include-dirs
	  -Wold-style-cast
	  -Woverloaded-virtual
	  -Wredundant-decls
	  -Wshadow
	  -Wstrict-overflow=5
	  -Wswitch-default
	  -Wundef
	  -Weffc++
	  -Winline
	  -Wswitch-enum)
endif()

# Source File Lists
file(GLOB_RECURSE ENGINE_SOURCES src/*.cpp)

# Targets
add_library(PhantomEngine ${ENGINE_SOURCES})

# Include Directories
target_include_directories(PhantomEngine PUBLIC include)

# Runtime Dependencies
target_link_libraries(PhantomEngine PUBLIC glfw3 glbinding glbinding-aux globjects)

# Preprocessor Definitions
target_compile_definitions(PhantomEngine PUBLIC gsl_CONFIG_CONTRACT_VIOLATION_THROWS)
if(NOT ENABLE_TESTS)
	target_compile_definitions(PhantomEngine PUBLIC DOCTEST_CONFIG_DISABLE)
endif()

# Build Examples
if(BUILD_EXAMPLES)
# TODO: add_subdirectory(examples)
endif()

# Build Tests
if(BUILD_TESTS)
	find_package(doctest REQUIRED)
	add_executable(run_tests src/test_runner.cpp)
	target_link_libraries(run_tests PRIVATE doctest::doctest PhantomEngine)
	add_test(NAME tests COMMAND run_tests)
endif()
