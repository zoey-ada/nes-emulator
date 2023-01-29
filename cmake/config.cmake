cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(build_dir ${CMAKE_BINARY_DIR})
set(bin_dir ${build_dir}/../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${bin_dir})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${bin_dir})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${bin_dir})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
