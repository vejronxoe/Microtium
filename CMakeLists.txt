cmake_minimum_required(VERSION 3.20)


set(CMAKE_CXX_STANDARD 17)



project("microtium")


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)


add_subdirectory(dependencies/glfw)		
add_subdirectory(dependencies/glad)				


file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/CloneTerraria/src/*.cpp")
add_executable("${CMAKE_PROJECT_NAME}"  ${MY_SOURCES})
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 20)
target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/CloneTerraria/src")



target_compile_definitions("${CMAKE_PROJECT_NAME}" PRIVATE GLEW_STATIC)
target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC GLFW_INCLUDE_NONE=1) 
target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE glad glfw )


set(MY_BUILD_TYPE "debug") # setting of the optimization level for the project, can be debug, release or debInfo


if(MY_BUILD_TYPE STREQUAL "debug")
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -O0 -g)
elseif(MY_BUILD_TYPE STREQUAL "release")
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -O3 -DNDEBUG)
elseif(MY_BUILD_TYPE STREQUAL "debInfo")
	target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE -O3 -g -DNDEBUG)
endif()
