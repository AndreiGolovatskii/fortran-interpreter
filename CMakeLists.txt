cmake_minimum_required(VERSION 3.0)

project(FortranInterpreter)

SET(CMAKE_CXX_FLAGS "-g -std=c++17")

include_directories(statements types interpreter driver grammar ${FortranInterpreter_SOURCE_DIR})

find_package(BISON 3.5 REQUIRED)
BISON_TARGET(
        MyParser
        grammar/parser.y
        ${FortranInterpreter_SOURCE_DIR}/grammar/parser.cpp
        DEFINES_FILE ${FortranInterpreter_SOURCE_DIR}/grammar/parser.hh
)

find_package(FLEX 2.6 REQUIRED)
FLEX_TARGET(
        MyScanner
        grammar/scanner.l
        ${FortranInterpreter_SOURCE_DIR}/grammar/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_library(
        Compiler
        driver/driver.cpp
        statements/statements.hh
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        types/types.hh
        types/types.cpp
)

add_executable(
        FortranInterpreter
        main.cpp
        ${HEADERS}
)

target_link_libraries(FortranInterpreter Compiler)
target_include_directories(FortranInterpreter PRIVATE ${FortranInterpreter_SOURCE_DIR})

enable_testing()
set(TEST_SOURCES ${FortranInterpreter_SOURCE_DIR}/tests/test_main)
add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${FortranInterpreter_SOURCE_DIR})
target_link_libraries(Tests Compiler)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} pthread)
