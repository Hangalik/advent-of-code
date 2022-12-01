add_library(input_selector input_selector.cpp)
target_compile_options(input_selector PRIVATE ${CXX_WARNING_FLAGS})
target_include_directories(input_selector PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")