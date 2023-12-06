# How to use:
#   mkdir build
#   cd build
#   cmake ..
#   make
#
cmake_minimum_required(VERSION 3.10)

project(FA
  LANGUAGES CXX C
)

find_package(Threads)


add_executable(testfa
  Automaton.cc
  testfa.cc
  googletest/googletest/src/gtest-all.cc
)

target_include_directories(testfa
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)

target_link_libraries(testfa
  PRIVATE
    Threads::Threads
)

target_compile_options(testfa
  PRIVATE
    "-Wall" "-Wextra" "-pedantic" "-g" "-O2"
)

set_target_properties(testfa
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
