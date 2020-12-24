cmake_minimum_required(VERSION 3.0)

project(FortranInterpreter)

SET(CMAKE_CXX_FLAGS "-g -std=c++17")

include_directories(driver grammar interpreter statements types)

add_subdirectory(types)
add_subdirectory(driver)

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
        LibInterpreter
        ${BISON_MyParser_OUTPUTS}
        ${FLEX_MyScanner_OUTPUTS}
        ${HEADERS}
)
target_link_libraries(LibInterpreter LibDriver)
target_link_libraries(LibInterpreter LibTypes)

add_executable(
        FortranInterpreter
        main.cpp
        ${HEADERS}
)


target_link_libraries(FortranInterpreter LibInterpreter)

enable_testing()
set(TEST_SOURCES ${FortranInterpreter_SOURCE_DIR}/tests/test_main)
add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${FortranInterpreter_SOURCE_DIR})
target_link_libraries(Tests LibInterpreter)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} Threads::Threads)
