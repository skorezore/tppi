cmake_minimum_required(VERSION 3.1.0)
project(tppi CXX)

option(BUILD_SHARED_LIBS "Build shared libraries.")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
        message(WARNING "GCC must be at least version 5!")
    endif()

    set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wall -Wextra -pedantic")

    if(NOT BUILD_SHARED_LIBS)
        set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wall -Wextra -pedantic -static")
    endif()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
        message(FATAL_ERROR "Clang must be at least version 3.4!")
    endif()

    set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wall -Wextra -pedantic")
    
    if(NOT BUILD_SHARED_LIBS)
        set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wall -Wextra -pedantic -static")
    endif()
else()
    message(WARNING "You are using an unsupported compiler!")
endif()

file(GLOB tppi_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp)
add_library (tppi ${tppi_source})
target_include_directories(tppi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
