cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
project(MapRenderer VERSION 0.0.1 LANGUAGES C)

set( HEADER_FILES "./include/mapRender.h;./include/mapRender.c")
add_executable(MapRenderer "main.c" ${HEADER_FILES} )
find_package(raylib 5.5 REQUIRED)
find_package(tmx REQUIRED)
target_include_directories(MapRenderer PUBLIC "./include")
target_link_libraries(MapRenderer PUBLIC tmx raylib)

if(UNIX)
  set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -g")
  find_package(X11 REQUIRED)
  target_link_libraries(MapRenderer PRIVATE X11)
endif()
