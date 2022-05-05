cmake_minimum_required(VERSION 3.10)

project(project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML REQUIRED system graphics window network)
include(default-options.cmake)
include_directories(include)
add_executable(project src/main.cpp src/client.cpp src/client_map.cpp src/server.cpp src/server_map.cpp src/declarations.cpp src/client_functions.cpp include/cl_sr_queue.h include/client_functions.h include/client_map.h include/client.h include/declarations.h include/server_map.h include/server.h)
target_link_libraries(project sfml-graphics sfml-system sfml-window sfml-network)
