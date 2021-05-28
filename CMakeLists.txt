cmake_minimum_required(VERSION 3.12)

project(phantom-drive-wt)

add_subdirectory(src)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR} CACHE STRING "Install path prefix, prepended onto install directories." FORCE)

set(ENABLE_TESTS OFF CACHE BOOL "Enables running unit tests with doctest; adds additional command line options")
set(ENABLE_NONPORTABLE_OPTIMIZATIONS OFF CACHE BOOL "Enables optimizations that may prevent the target from working on other systems")

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
		-pedantic
	  -Wall
	  -Wextra
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
