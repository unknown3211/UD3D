cmake_minimum_required(VERSION 3.28)

project(UD3D)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "include/*.h")
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${MY_HEADERS} ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_NO_EXCEPTIONS ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_STATIC_LIB ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(
    sol2
    URL https://github.com/ThePhD/sol2/archive/refs/tags/v3.3.0.zip
)

FetchContent_Declare(
    assimp
    URL https://github.com/assimp/assimp/archive/refs/tags/v6.0.2.zip
)

FetchContent_MakeAvailable(json)
FetchContent_MakeAvailable(sol2)
FetchContent_MakeAvailable(assimp)

add_subdirectory(libs/glad)
add_subdirectory(libs/glfw-3.4)
add_subdirectory(libs/glm)
add_subdirectory(libs/stb)
add_subdirectory(libs/openal-soft)
add_subdirectory(libs/libsndfile)
add_subdirectory(libs/freetype)
add_subdirectory(libs/lua)
add_subdirectory(libs/imgui-docking)

target_link_libraries(${PROJECT_NAME} PUBLIC 
  glad   
  glfw
  glm 
  stb
  OpenAL 
  SndFile::sndfile 
  freetype 
  lua_static 
  nlohmann_json::nlohmann_json 
  sol2
  assimp
  imgui
)