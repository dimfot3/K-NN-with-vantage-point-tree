cmake_minimum_required(VERSION 3.12)

project(knn_w_vantage LANGUAGES CUDA C CXX)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

include_directories(include)

add_library(utils STATIC src/utils.c)
target_link_libraries(utils PUBLIC m)

add_library(ser_vp_tree STATIC src/serial_vp_tree.c)
target_link_libraries(ser_vp_tree PUBLIC utils)

add_executable(main_cpu_program src/main_cpu_program.c)
target_link_libraries(main_cpu_program utils ser_vp_tree)


file(GLOB SCRIPTS
  "scripts/*.sh"
)
file(COPY ${SCRIPTS} DESTINATION ${CMAKE_BINARY_DIR})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif(BUILD_TESTING)
