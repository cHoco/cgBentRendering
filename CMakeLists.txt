cmake_minimum_required (VERSION 2.8)
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
include(ExternalProject)
project (cgBentRendering)

add_executable(cgBentRendering
    cgBentRendering.cpp
    utility/Shader.cpp
)

# Use c++11 in all cases.
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# CMake modules path.
SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake/modules
    ${CMAKE_MODULE_PATH}
)

# OpenGL
find_package (OpenGL REQUIRED)
if (OPENGL_FOUND)
    link_libraries(${OPENGL_LIBRARIES})
endif()

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

# GLFW
find_package(GLFW3 REQUIRED glfw3)
if (GLFW3_FOUND)
    include_directories(${GLFW3_INCLUDE_PATH})
    link_libraries(${GLFW3_LIBRARIES})
endif()

# GLM
find_package (GLM REQUIRED)
if (GLM_FOUND)
    include_directories(${GLM_INCLUDE_PATH})
    link_libraries(${GLM_LIBRARIES})
endif()

# Assimp
find_package (ASSIMP REQUIRED)
if (ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    link_libraries(${ASSIMP_LIBRARIES})
endif()

# SOIL
ExternalProject_Add(
    SOIL
    PREFIX ${PROJECT_SOURCE_DIR}/Externals/SOIL
    GIT_REPOSITORY https://github.com/kbranigan/Simple-OpenGL-Image-Library.git
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_SOURCE_DIR}/Externals/SOIL
    UPDATE_COMMAND ""
    LOG_DOWNLOAD ON
    )
set(soilLib ${PROJECT_SOURCE_DIR}/Externals/SOIL)
include_directories(${soilLib}/include)
target_link_libraries(cgBentRendering ${soilLib}/lib/libSOIL.a)

add_library(FileUtils "${PROJECT_SOURCE_DIR}/utility/fileUtils.cpp")
add_library(GLFWUtils "${PROJECT_SOURCE_DIR}/utility/glfwUtils.cpp")
include_directories ("${PROJECT_SOURCE_DIR}/utility")
target_link_libraries (cgBentRendering FileUtils GLFWUtils)

