cmake_minimum_required(VERSION 3.12)
project(knn_w_vantage)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

include_directories(include)

add_library(utils src/utils.c)

add_executable(main_cpu_program src/main_cpu_program.c)
target_link_libraries(main_cpu_program utils)


file(GLOB SCRIPTS
  "scripts/*.sh"
)
file(COPY ${SCRIPTS} DESTINATION ${CMAKE_BINARY_DIR})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif(BUILD_TESTING)
