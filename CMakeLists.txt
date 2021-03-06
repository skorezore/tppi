cmake_minimum_required(VERSION 3.1.0)
project(tppi CXX)

option(BUILD_SHARED_LIBS "Build shared libraries.")

macro(apply_compiler_flags)
    set(CMAKE_CXX_FLAGS "-std=c++14 -O3 -Wall -Wextra -pedantic")

    if(NOT BUILD_SHARED_LIBS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    endif()
endmacro(apply_compiler_flags)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 4.9)
        message(WARNING "GCC must be at least version 4.9!")
    endif()

    apply_compiler_flags()
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.6)
        message(FATAL_ERROR "Clang must be at least version 3.6!")
    endif()

    apply_compiler_flags()
else()
    message(WARNING "You are using an unsupported compiler!")
endif()

file(GLOB_RECURSE tppi_source ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(tppi ${tppi_source})
target_include_directories(tppi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(GLFW_BUILD_DOCS FALSE CACHE BOOL "")
set(GLFW_BUILD_EXAMPLES FALSE CACHE BOOL "")
set(GLFW_BUILD_TESTS FALSE CACHE BOOL "")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/glfw/include)
target_link_libraries(tppi glfw ${GLFW_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/glm)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/tdpi)
target_link_libraries(tppi tdpi)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/deps/tinyobjloader)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/deps/tinyobjloader)
target_link_libraries(tppi tinyobjloader)
