cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
project(TiledPlayground VERSION 0.0.1 LANGUAGES C)

add_executable(Test "main.c")
find_package(raylib 5.5 REQUIRED)
find_package(tmx REQUIRED)
target_link_libraries(Test PRIVATE tmx raylib)

if(UNIX)
  set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -g")
  find_package(X11 REQUIRED)
  target_link_libraries(Test PRIVATE X11)
endif()
