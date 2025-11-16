cmake_minimum_required(VERSION 3.20)

project(ds_and_algo)

set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

# Clang tidy checks
option(ENABLE_CLANG_TIDY "Run clang-tidy during build (off by default)" OFF)

if(ENABLE_CLANG_TIDY)
	find_program(CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-18)
	if(CLANG_TIDY_EXE)
		message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
		if(NOT DEFINED CLANG_TIDY_CHECKS)
			set(CLANG_TIDY_CHECKS "-checks=*,-clang-analyzer-*" CACHE STRING "Checks for clang-tidy")
		endif()
		set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};${CLANG_TIDY_CHECKS}")
	else()
		message(WARNING "clang-tidy not found; please install clang-tidy to enable static analysis")
	endif()
endif()

# Compile ds/span/main.cpp
add_executable(ds_and_algo ds/span/main.cpp)
target_include_directories(ds_and_algo PRIVATE ds/span)
target_compile_options(ds_and_algo PRIVATE -Wall -Wextra -Wpedantic -Werror)