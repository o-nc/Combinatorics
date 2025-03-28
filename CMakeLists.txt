cmake_minimum_required(VERSION 3.30)
project(Combinatorics)

set(CMAKE_CXX_STANDARD 23)

add_library(Combinatorics STATIC library.cpp)
target_include_directories(Combinatorics PUBLIC include
        ${PROJECT_SOURCE_DIR}/src)

add_subdirectory(external/googletest)
add_executable(unittests
        tests/LockfreeCounter.test.cpp)
target_link_libraries(unittests gtest gtest_main Combinatorics)
target_include_directories(unittests PUBLIC include
        ${PROJECT_SOURCE_DIR}/src)