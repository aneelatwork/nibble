cmake_minimum_required(VERSION 4.0.3)

set(TEST_TARGET_NAME nibble_test)

if (MSVC)
    add_compile_options("/EHsc")
endif (MSVC)

find_package(GTest CONFIG REQUIRED)
add_executable(${TEST_TARGET_NAME})
target_include_directories(${TEST_TARGET_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        ${CMAKE_PROJECT_NAME}
        GTest::gtest_main
)

target_sources(${TEST_TARGET_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp
)
include(GoogleTest)
gtest_discover_tests(${TEST_TARGET_NAME})
