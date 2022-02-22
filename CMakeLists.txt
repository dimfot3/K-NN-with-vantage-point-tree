cmake_minimum_required(VERSION 3.12)

project(knn_w_vantage LANGUAGES C CXX)
find_package(OpenMP)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS} -O3 -fopenmp")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -O3")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

include_directories(include)

#basic utilities
add_library(utils STATIC src/utils.c)
target_link_libraries(utils PUBLIC m)

#basic utilities
add_library(queue STATIC src/Queue.c)

#serial vantage point creation
add_library(ser_vp_tree STATIC src/serial_vp_tree.c)
target_link_libraries(ser_vp_tree PUBLIC utils)

#knn search function
add_library(knn_search STATIC src/search_knn.c)
target_link_libraries(knn_search PUBLIC utils queue)

#openmp implementation
add_library(openmp_vp_tree STATIC src/open_mp_tree.c)
target_link_libraries(openmp_vp_tree PUBLIC utils)

#mixed implementation
add_library(mixed_vp_tree STATIC src/mixed_tree.c)
target_link_libraries(mixed_vp_tree PUBLIC utils)

#main function for serial, openmp, opencilk versions
add_executable(main_cpu_program src/main_cpu_program.c)
target_link_libraries(main_cpu_program utils ser_vp_tree knn_search queue openmp_vp_tree mixed_vp_tree)

file(GLOB SCRIPTS
  "scripts/*.sh"
)
file(COPY ${SCRIPTS} DESTINATION ${CMAKE_BINARY_DIR})

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif(BUILD_TESTING)
