cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(advent-of-code-2023 LANGUAGES C CXX)

set(CXX_WARNING_FLAGS -Wall -Wextra -Wdeprecated -Wno-unused -pedantic)

find_package(Boost REQUIRED)
message(STATUS "Found boost library ${Boost_VERSION_STRING}")

function(add_new_day day)
	add_executable(${day} main.cpp)
	target_compile_options(${day} PRIVATE ${CXX_WARNING_FLAGS} "-fno-rtti")
	target_link_libraries(${day} PRIVATE input_selector Boost::boost)
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
add_subdirectory(day-08)
add_subdirectory(day-09)
add_subdirectory(day-10)
add_subdirectory(day-11)
add_subdirectory(day-12)
