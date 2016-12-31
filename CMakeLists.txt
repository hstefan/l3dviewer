cmake_minimum_required(VERSION 3.5)

project(l3dviewer)

# Setup glad dependency
set(GLAD_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/external/glad/include")
set(GLAD_SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")

add_library(glad STATIC
  ${GLAD_SOURCES})

target_include_directories(glad
  PUBLIC ${GLAD_INCLUDE_DIRS})

# Setup GLFW dependency
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

add_executable(l3dviewer
  src/main.cpp)

set_property(TARGET l3dviewer PROPERTY CXX_STANDARD 14)

add_custom_command(
  TARGET l3dviewer PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/files $<TARGET_FILE_DIR:l3dviewer>/files)

target_link_libraries(l3dviewer glad glfw)
