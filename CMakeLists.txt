cmake_minimum_required(VERSION 3.9.4)
project(learn_stl)

set(CMAKE_CXX_STANDARD 17)

find_package(GTest      REQUIRED)
find_package(Threads    REQUIRED)

include(cmake/sanitizers.cmake)

set(include_paths   ${PROJECT_SOURCE_DIR})
link_directories(/usr/local/lib/)

file(GLOB   test_cxx_source_files           ${PROJECT_SOURCE_DIR}/learn_stl/test/*.cc)
add_executable(test_learn_stl               ${test_cxx_source_files})
target_include_directories(test_learn_stl   PUBLIC ${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
target_link_libraries(test_learn_stl        gtest gmock gtest_main ${CMAKE_THREAD_LIBS_INIT})
add_test(test_learn_stl     test_learn_stl)

enable_testing()

file(GLOB_RECURSE all_cxx_source_files ${PROJECT_SOURCE_DIR}/*.cc ${PROJECT_SOURCE_DIR}/*.h)

find_program(clang_format "clang-format")
if(clang_format)
    add_custom_target(clang-format COMMAND ${clang_format} -i -style=file ${all_cxx_source_files})
endif()

find_program(clang_tidy "clang-tidy")
if(clang_tidy)
    add_custom_target(clang-tidy COMMAND ${clang_tidy} ${all_cxx_source_files} -checks=* -- -std=c++1z -I${include_paths})
endif()