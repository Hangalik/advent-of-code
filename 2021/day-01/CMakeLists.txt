add_executable(day-01-1 1.cpp)
target_compile_options(day-01-1 PRIVATE ${CXX_WARNING_FLAGS})
target_link_libraries(day-01-1 PRIVATE input_selector)
set_target_properties(day-01-1 PROPERTIES OUTPUT_NAME 1)

add_executable(day-01-2 2.cpp)
target_compile_options(day-01-2 PRIVATE ${CXX_WARNING_FLAGS})
target_link_libraries(day-01-2 PRIVATE input_selector)
set_target_properties(day-01-2 PROPERTIES OUTPUT_NAME 2)