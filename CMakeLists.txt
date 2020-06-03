cmake_minimum_required(VERSION 3.0)
project(snake_game)

include_directories(include)

file(GLOB SOURCES "src/*.c")

add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE allegro allegro_font allegro_primitives)

