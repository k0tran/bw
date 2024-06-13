cmake_minimum_required(VERSION 3.21)
project(bw VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0135 NEW)

set(DEFAULT_BUILD_TYPE "Release")
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
    set(DEFAULT_BUILD_TYPE "Debug")
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(FetchContent)

FetchContent_Declare(
    datatype99
    URL https://github.com/Hirrolot/datatype99/archive/refs/tags/v1.6.4.tar.gz
)

FetchContent_MakeAvailable(datatype99)

FetchContent_Declare(
    interface99
    URL https://github.com/Hirrolot/interface99/archive/refs/tags/v1.0.1.tar.gz
)

FetchContent_MakeAvailable(interface99)

add_executable(${PROJECT_NAME}
    src/main.c
    src/args.c
    src/common.c
    src/lexer.c
    src/ast.c
)
target_compile_definitions(${PROJECT_NAME} PRIVATE VERSION="${CMAKE_PROJECT_VERSION}")
target_link_libraries(${PROJECT_NAME} interface99 datatype99)

# Disable full macro expansion backtraces for Metalang99.
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -fmacro-backtrace-limit=1)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -ftrack-macro-expansion=0)
endif()
