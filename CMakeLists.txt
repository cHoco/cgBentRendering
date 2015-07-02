cmake_minimum_required (VERSION 2.8)
project (cgBentRendering)

add_executable(cgBentRendering
    cgBentRendering.cpp
    utility/EinApplication.cpp
    utility/EinWindow.cpp
    utility/EinInputManager.cpp
    utility/Shader.cpp
    utility/Texture.cpp
    utility/Camera.cpp
    utility/Model.cpp
    utility/Mesh.cpp
    utility/Framebuffer.cpp
    utility/lodepng.cpp
)

# file(INSTALL
#     textures
#     shaders
#     DESTINATION
#     ${PROJECT_BINARY_DIR}
# )

install( DIRECTORY "shaders" DESTINATION ${PROJECT_BINARY_DIR})
install( DIRECTORY "models" DESTINATION ${PROJECT_BINARY_DIR})

# Use c++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# CMake modules path.
SET(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/modules
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
find_package(GLFW REQUIRED)
if (GLFW_FOUND)
    include_directories(${GLFW_INCLUDE_PATH})
    link_libraries(${GLFW_LIBRARIES})
endif()

# GLM
find_package (GLM REQUIRED)
if (GLM_FOUND)
    include_directories(${GLM_INCLUDE_PATH})
endif()

# Assimp
find_package (ASSIMP REQUIRED)
if (ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
    link_libraries(${ASSIMP_LIBRARIES})
endif()

add_library(FileUtils "${PROJECT_SOURCE_DIR}/utility/fileUtils.cpp")
include_directories ("${PROJECT_SOURCE_DIR}/utility")
target_link_libraries (cgBentRendering FileUtils)
