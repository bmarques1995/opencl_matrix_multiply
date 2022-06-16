cmake_minimum_required(VERSION 3.20)

project(ocl_matrix_multiply)

set(CMAKE_CXX_STANDARD 17)

add_executable(ocl_matrix_multiply src/entry.cc src/Utils/FileHandler.cc src/Utils/FileHandler.hh)

add_subdirectory(vendor/opencl_wrapper)

file(GLOB_RECURSE CL_KERN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cl")

foreach(kernel IN LISTS CL_KERN)
    file(COPY ${kernel} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

target_include_directories(ocl_matrix_multiply PRIVATE ./src ./vendor/eigen3 ./vendor/opencl_wrapper/vendor/opencl_headers ./vendor/opencl_wrapper/src ./vendor/opencl_wrapper/vendor/opencl_hpp)
target_link_libraries(ocl_matrix_multiply PRIVATE opencl_wrapper)