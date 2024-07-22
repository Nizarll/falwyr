cmake_minimum_required(VERSION 3.30.0)
set(CMAKE_CXX_STANDARD 23)
project(falwyr)
include(FetchContent)


set(RAYLIB_VERSION 5.0)

FetchContent_Declare(
  raylib
  URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
  FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)
FetchContent_MakeAvailable(raylib)

option(TILE_EDITOR "Build tile editor" OFF)
if(TILE_EDITOR)
    add_executable(tile_editor src/tile_editor.cpp)
    target_link_libraries(tile_editor pthread)
    target_link_libraries(tile_editor raylib)
    message("building tile editor!")
else()
    set(SOURCE_FILES src/main.cpp src/entity.cpp)
    add_executable(app ${SOURCE_FILES})
    target_link_libraries(app m)
    target_link_libraries(app raylib)
    message("building game!")
endif()
