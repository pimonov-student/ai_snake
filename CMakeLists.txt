cmake_minimum_required(VERSION 3.25.2 FATAL_ERROR)

set(PROJECT_NAME ai_snake)
set (CMAKE_CXX_STANDARD 17)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/shader/shader.cpp
	src/shader/shader.h
	src/texture/texture.cpp
	src/texture/texture.h
	src/texture/stb_image.h)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)