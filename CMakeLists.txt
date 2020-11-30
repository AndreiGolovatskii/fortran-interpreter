cmake_minimum_required(VERSION 3.17)

project(ParserExample)

SET(CMAKE_CXX_FLAGS "-g -std=c++17")
add_definitions(-DYYDEBUG=1)

set(
        HEADERS
        ${ParserExample_SOURCE_DIR}/driver.hh
        ${ParserExample_SOURCE_DIR}/parser.hh
        ${ParserExample_SOURCE_DIR}/operators.hh
)

find_package(BISON 3.5 REQUIRED)
BISON_TARGET(
        MyParser
        parser.y
        ${ParserExample_SOURCE_DIR}/parser.cpp
        DEFINES_FILE ${ParserExample_SOURCE_DIR}/parser.hh
)

find_package(FLEX 2.6 REQUIRED)
FLEX_TARGET(
        MyScanner
        scanner.l
        ${ParserExample_SOURCE_DIR}/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_library(
        Compiler
        driver.cpp
        operators.hh
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        types.hh types.cpp)

add_executable(
        ParserExample
        main.cpp
        ${HEADERS}
)

target_link_libraries(ParserExample Compiler)
target_include_directories(ParserExample PRIVATE ${ParserExample_SOURCE_DIR})

enable_testing()
set(TEST_SOURCES ${ParserExample_SOURCE_DIR}/tests/test_main)
add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${ParserExample_SOURCE_DIR})
target_link_libraries(Tests Compiler)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} pthread)
