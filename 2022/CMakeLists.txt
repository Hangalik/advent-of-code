cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(advent-of-code-2022 LANGUAGES C CXX)

set(CXX_WARNING_FLAGS -Wall -Wextra -Wdeprecated -pedantic)

function(add_new_day day)
	add_executable(${day} main.cpp)
	target_compile_options(${day} PRIVATE ${CXX_WARNING_FLAGS})
	target_link_libraries(${day} PRIVATE input_selector)
	set_target_properties(${day} PROPERTIES OUTPUT_NAME main)
endfunction()

add_subdirectory(input_selector)

add_subdirectory(day-01)
add_subdirectory(day-02)
add_subdirectory(day-03)
add_subdirectory(day-04)
add_subdirectory(day-05)
add_subdirectory(day-06)
add_subdirectory(day-07)
