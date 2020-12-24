cmake_minimum_required(VERSION 3.0)

project(FortranInterpreter)

SET(CMAKE_CXX_FLAGS "-g -std=c++17")

include_directories(driver grammar interpreter statements types)

add_subdirectory(grammar)
add_subdirectory(types)
add_subdirectory(driver)

add_library(
        LibInterpreter
        INTERFACE
)
target_link_libraries(LibInterpreter INTERFACE LibDriver LibTypes LibGrammar)

add_executable(
        FortranInterpreter
        main.cpp
        ${HEADERS}
)


target_link_libraries(FortranInterpreter LibInterpreter)

enable_testing()
set(TEST_SOURCES ${FortranInterpreter_SOURCE_DIR}/tests/test_main.cpp)
add_executable(Tests ${TEST_SOURCES})
target_include_directories(Tests PRIVATE ${FortranInterpreter_SOURCE_DIR})
target_link_libraries(Tests LibInterpreter)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(Tests ${GTEST_BOTH_LIBRARIES} ${GTEST_LIBRARIES} Threads::Threads)
