cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(advent-of-code-2021 LANGUAGES C CXX)

set(CXX_WARNING_FLAGS -Wall -Wextra -Wdeprecated -pedantic)

function(add_new_day day)
	add_executable(${day}-1 1.cpp)
	target_compile_options(${day}-1 PRIVATE ${CXX_WARNING_FLAGS})
	target_link_libraries(${day}-1 PRIVATE input_selector)
	set_target_properties(${day}-1 PROPERTIES OUTPUT_NAME 1)

	add_executable(${day}-2 2.cpp)
	target_compile_options(${day}-2 PRIVATE ${CXX_WARNING_FLAGS})
	target_link_libraries(${day}-2 PRIVATE input_selector)
	set_target_properties(${day}-2 PROPERTIES OUTPUT_NAME 2)
endfunction()

add_subdirectory(input_selector)

add_subdirectory(day-01)
add_subdirectory(day-02)