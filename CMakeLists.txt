cmake_minimum_required(VERSION 3.31)
project(learnOpenGl)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        GIT_TAG master
)
FetchContent_MakeAvailable(assimp)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(OpenSSL REQUIRED)


add_executable(${PROJECT_NAME}

        src/render/AffineTransform.cpp
        src/render/CheckpointRender.cpp
        src/render/Player.cpp
        src/render/Bottom.cpp
        src/render/WallRender.cpp

        src/physics/CheckpointPhysics.cpp
        src/physics/AABB.cpp
        src/physics/WallPhysics.cpp

        src/main.cpp
        src/thirdparty/stb_image/stb.cpp
        src/thirdparty/glad/glad.c

        src/websocket/WebSocketClient.cpp
        src/physics/raycast/RayCaster.cpp
        src/physics/raycast/RayCaster.h
        src/RL/RLTrainData.cpp
        src/RL/RLTrainData.h
        src/RL/RlResponse.cpp
        src/RL/RlResponse.h
        src/render/PlayerMovement.h
)

target_include_directories(${PROJECT_NAME} PRIVATE
        external/Simple-WebSocket-Server
        external/boost/asio/include
        external/boost/config/include
        external/boost/assert/include
        external/boost/throw_exception/include
        external/boost/system/include
        external/json
        recources
)

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENSSL_LIBRARIES})

target_link_libraries(${PROJECT_NAME} PRIVATE assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)
