cmake_minimum_required(VERSION 3.10)
project(Text_Editor)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 3.3 REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find FreeType
find_package(Freetype REQUIRED)

# Add include directories
include_directories(${OPENGL_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${FREETYPE_INCLUDE_DIRS} include)

# Add source files
file(GLOB SOURCES "src/*.cpp")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw GLEW::GLEW Freetype::Freetype)

