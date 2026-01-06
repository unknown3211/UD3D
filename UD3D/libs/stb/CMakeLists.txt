cmake_minimum_required(VERSION 3.1)
project(stb)

add_library(stb_image STATIC src/stb_image.cpp)
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(stb_truetype STATIC src/stb_truetype.cpp)
target_include_directories(stb_truetype PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(stb INTERFACE)
target_link_libraries(stb INTERFACE stb_image stb_truetype)