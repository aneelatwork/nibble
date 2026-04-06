cmake_minimum_required(VERSION 4.0.3)

# set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
# set(CMAKE_CXX_MODULE_STD ON)
#
project(nibble VERSION 00.00.01 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "either None Debug Release RelWithDebInfo MinSizeRel" FORCE)
set(CXX_SCAN_FOR_MODULES ON)

if (MSVC)
    add_compile_options("/EHsc")
endif (MSVC)

add_library(${CMAKE_PROJECT_NAME})
#add_executable(${CMAKE_PROJECT_NAME})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_sources(${CMAKE_PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/raks/nibble/nibble.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/raks/nibble/iterator.hpp

    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/nibble.cpp

    # PUBLIC
    #     FILE_SET CXX_MODULES
    #     FILES
    #         ${CMAKE_CURRENT_SOURCE_DIR}/bcd.ccm
)

# Use 'conan <command> -c tools.build:skip_test=True ...' to skip test

if (NOT BUILD_TESTING STREQUAL OFF)
    enable_testing()
    add_subdirectory(test)
endif()
