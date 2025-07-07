cmake_minimum_required(VERSION 3.11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
project(TilemapCollision VERSION 0.0.1 LANGUAGES C)

set( TILEMAP_HEADER_FILES "./include/mapRender.h;./include/mapRender.c;")
set( PLAYER_HEADER_FILES "./include/player.h;./include/player.c")
add_executable(app "main.c" ${TILEMAP_HEADER_FILES} ${PLAYER_HEADER_FILES} )
find_package(raylib 5.5 REQUIRED)
find_package(tmx REQUIRED)
target_include_directories(app PUBLIC "./include")
target_link_libraries(app PUBLIC tmx raylib)

if(UNIX)
  set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -g")
  find_package(X11 REQUIRED)
  target_link_libraries(app PRIVATE X11)
endif()
