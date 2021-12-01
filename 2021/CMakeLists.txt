cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(advent-of-code-2021 LANGUAGES C CXX)

set(CXX_WARNING_FLAGS -Wall -Wextra -Wdeprecated -pedantic)

add_subdirectory(input_selector)

add_subdirectory(day-01)