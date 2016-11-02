cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)
set(CMAKE_BUILD_TYPE "Release")
find_package(HPX)

include_directories(${HPX_INCLUDE_DIRS})

add_library(test_library SHARED test_library.cpp)

target_link_libraries(
  test_library
  hpx
  hpx_init
  ${HPX_LIBRARIES})

add_executable(main main.cpp test_object_a.cpp)

target_link_libraries(
  main
  hpx
  hpx_init
  ${HPX_LIBRARIES}
  test_library)
