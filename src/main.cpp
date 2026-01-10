#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

#include "physics/CheckpointPhysics.h"
#include "thirdparty/stb_image/stb_image.h"
#include "thirdparty/glad/glad.h"

#include "render/Player.h"
#include "render/Bottom.h"
#include "render/WallRender.h"
#include "render/AffineTransform.h"
#include "render/CheckpointRender.h"


constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;
constexpr int FPS = 61;

float last_x = static_cast<float>(WIDTH) / 2;
float last_y = static_cast<float>(HEIGHT) / 2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Player player(glm::vec3(-4.0f, 2.0f, -4.0f));

std::string light_vert_path = "../src/shaders/light_cube.vert";
std::string light_frag_path = "../src/shaders/light_cube.frag";
std::string color_vert_path = "../src/shaders/colors.vert";
std::string color_frag_path = "../src/shaders/colors.frag";

std::string bottom_vertex_path = "../src/shaders/bottom.vert";
std::string bottom_fragment_path = "../src/shaders/bottom.frag";

std::string wall_vertex_path = "../src/shaders/wall.vert";
std::string wall_fragment_path = "../src/shaders/wall.frag";

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.processCameraPositionMovement(FORWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.processCameraPositionMovement(BACKWARD);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.processCameraPositionMovement(LEFT);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.processCameraPositionMovement(RIGHT);
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    float xoffset = xpos - last_x;
    float yoffset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;
    player.processMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    player.processMouseScroll(yoffset);
}

void displayFPS(double dt, GLFWwindow &window) {
    const double fps = 1.0 / dt;
    std::string title = std::string("Tykhon - 3D Renderer - FPS: ") + std::to_string(static_cast<int>(fps));
    glfwSetWindowTitle(&window, title.c_str());
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "3D-Renderer", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    std::vector<glm::vec3> floorPositions{
        glm::vec3(0.0f),
        glm::vec3(-8.0f, 0.0f, 0.0f)
    };

    std::vector<AffineTransform> affine_transforms{
        AffineTransform(
            glm::vec3(-4.0f, 1.9f, 0.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.829228f)
        ),
        AffineTransform(
            glm::vec3(-12.0f, 1.9f, 0.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.829228f)
        ),
        AffineTransform(
            glm::vec3(-4.0f, 1.9f, -8.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.829228f)
        ),
        AffineTransform(
            glm::vec3(-12.0f, 1.9f, -8.0f),
            glm::vec3(90.0f, 0.0f, 0.0f),
            glm::vec3(0.829228f)
        ),
        AffineTransform(
            glm::vec3(-16.0f, 1.9f, -4.0f),
            glm::vec3(90.0f, 0.0f, 90.0f),
            glm::vec3(0.829228f)
        ),
        AffineTransform(
            glm::vec3(0.0f, 1.9f, -4.0f),
            glm::vec3(90.0f, 0.0f, 90.0f),
            glm::vec3(0.829228f)
        )
    };
    Bottom bottom(floorPositions,
                  "../resources/checkered_tile_floor.glb",
                  bottom_vertex_path,
                  bottom_fragment_path);

    WallRender wall_render("../resources/grungy_brick_wall.glb",
                           wall_vertex_path,
                           wall_fragment_path);


    stbi_set_flip_vertically_on_load(false);

    CheckpointRender checkpoint_render("../resources/bfbbr_-_checkpoint.glb",
                                       wall_vertex_path,
                                       wall_fragment_path);


    AABB room_aabb (
        glm::vec3(-1.0f, 0.5f, -1.0f),
        glm::vec3(-10.0f, 0.5f, -7.0f)
        );

    CheckpointPhysics checkpoint_physics(AffineTransform(
                                             glm::vec3(-3.0f, 0.5f, -3.0f),
                                             glm::vec3(0.0f),
                                             glm::vec3(0.2f)
                                         ),
                                         checkpoint_render.getAABB(),
                                         room_aabb
    );

    std::vector<WallPhysics> wall_physicses;
    wall_physicses.reserve(affine_transforms.size());

    for (auto const &affine_transform: affine_transforms) {
        wall_physicses.emplace_back(affine_transform, wall_render.getAABB());
    }


    // main loop
    double last_time = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        double now = glfwGetTime();
        double dt = now - last_time;
        if (dt >= 1.0 / FPS) {
            displayFPS(dt, *window);
            last_time = now;
        } else {
            continue;
        }
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.updatePlayer(bottom.getCommonY(), dt, wall_physicses, checkpoint_physics);

        glm::mat4 view = player.getView();
        glm::mat4 projection = glm::perspective(glm::radians(player.getZoom()),
                                                static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
                                                0.1f,
                                                100.0f);


        bottom.draw(view, projection);

        wall_render.draw(view, projection, wall_physicses);

        checkpoint_render.draw(view, projection, checkpoint_physics);

        glfwSwapBuffers(window);
    }

    return 0;
}
