cmake_minimum_required(VERSION 3.20)

project(ocl_matrix_multiply)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/opencl_wrapper)

file(GLOB_RECURSE CL_KERN RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cl")
file(GLOB_RECURSE CL_PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR} "src/*.cc")
file(GLOB_RECURSE CL_PROJECT_HEADERS ${CMAKE_CURRENT_SOURCE_DIR} "src/*.hh")

add_executable(ocl_matrix_multiply ${CL_PROJECT_SOURCES} ${CL_PROJECT_HEADERS})

foreach(kernel IN LISTS CL_KERN)
    file(COPY ${kernel} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach()

target_include_directories(ocl_matrix_multiply PRIVATE ./src ./vendor/eigen3 ./vendor/opencl_wrapper/vendor/opencl_headers ./vendor/opencl_wrapper/src ./vendor/opencl_wrapper/vendor/opencl_hpp)
target_link_libraries(ocl_matrix_multiply PRIVATE opencl_wrapper)