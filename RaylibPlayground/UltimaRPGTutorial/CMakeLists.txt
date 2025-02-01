cmake_minimum_required(VERSION 3.11)
project(Tutorial)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")
add_executable(Tutorial "main.c")

# Dependencies
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_MakeAvailable(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
  endif()
endif()

target_link_libraries(Tutorial PRIVATE raylib)

if(UNIX)
  set(CMAKE_CXX_FLAGS "-O1 -Wall -std=c99 -Wno-missing-braces")
  find_package(X11 REQUIRED)
  target_link_libraries(Tutorial PRIVATE X11)
endif()
