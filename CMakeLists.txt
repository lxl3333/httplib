cmake_minimum_required(VERSION 3.1)

project(ftpproject)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/base)
add_subdirectory(src/server)
add_subdirectory(src/client)
add_subdirectory(test)
