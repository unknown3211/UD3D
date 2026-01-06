cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(PRODUCTION_BUILD "Make this a production build!" OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>")
endif()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

if(MSVC) 
    add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()

project(project1)

file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES} ${MY_HEADERS})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

if(PRODUCTION_BUILD)
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") 

	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1) 

else()
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/")
	target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0) 
endif()

if(MSVC)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
    #set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console	
endif()


if(UNIX AND NOT APPLE)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE m pthread dl rt X11)
endif()

add_subdirectory(libs/UD3D)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE UD3D)