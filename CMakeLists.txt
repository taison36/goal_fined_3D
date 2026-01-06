cmake_minimum_required(VERSION 3.31)
project(learnOpenGl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        3.4
)
FetchContent_MakeAvailable(glfw)
FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG  master
)
FetchContent_MakeAvailable(assimp)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)


add_executable(${PROJECT_NAME}
        src/Player.h
        src/Player.cpp
        src/Bottom.cpp
        src/Bottom.h
        src/main.cpp
        src/thirdparty/stb_image/stb.cpp
        src/thirdparty/glad/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE recources)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
