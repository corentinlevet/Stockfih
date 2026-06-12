cmake_minimum_required(VERSION 3.20)

project(Stockfih
  VERSION 0.1.0
  DESCRIPTION "A chess engine and raylib GUI written from scratch in C++"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(STOCKFIH_BUILD_TESTS "Build the unit tests" ON)
option(STOCKFIH_BUILD_APP "Build the raylib GUI application" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Dependencies)

# Headless chess engine -- the heart of the project, no graphics dependency.
add_subdirectory(engine)
