cmake_minimum_required(VERSION 3.9)
project(libtp)

set(CMAKE_CXX_STANDARD 17)

add_executable(libtp main.cpp task.h task.cpp lambda_task.h lambda_task.cpp future_task.h future_task.impl.h thread_pool.h thread_pool.cpp)
target_link_libraries(libtp pthread)