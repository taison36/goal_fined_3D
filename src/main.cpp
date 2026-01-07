#include <iostream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>

#include "thirdparty/stb_image/stb_image.h"
#include "thirdparty/glad/glad.h"

#include "Player.h"
#include "Bottom.h"
#include "Wall.h"


constexpr int WIDTH = 1024;
constexpr int HEIGHT = 768;
constexpr int FPS = 61;

float last_x = static_cast<float>(WIDTH) / 2;
float last_y = static_cast<float>(HEIGHT) / 2;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Player player(glm::vec3(0.0f, 2.0f, 0.0f));

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

void processInput(GLFWwindow *window, float delta_time) {
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        player.processCameraPositionMovement(FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        player.processCameraPositionMovement(BACKWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player.processCameraPositionMovement(LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player.processCameraPositionMovement(RIGHT, delta_time);
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

    std::vector<glm::vec3> wall_positions_0{
        glm::vec3(0.0f, -0.1f, 0.0f),
        glm::vec3(-8.0f, -0.1f, 0.0f)
    };

    std::vector<glm::vec3> wall_positions_1{
        glm::vec3(0.0f, -0.1f, 0.0f)
    };

    std::vector<glm::vec3> wall_positions_2{
        glm::vec3(0.0f, -0.1f, -8.0f),
        glm::vec3(-8.0f, -0.1f, -8.0f)
    };

    std::vector<glm::vec3> wall_positions_3{
        glm::vec3(-8.0f, -0.1f, 0.0f)
    };

    Bottom bottom(floorPositions,
                  "../resources/checkered_tile_floor.glb",
                  bottom_vertex_path,
                  bottom_fragment_path);

     Wall wall("../resources/backpack/backpack.obj",
               wall_vertex_path,
               wall_fragment_path);

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
        processInput(window, dt);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //player.updatePlayer(bottom.getCommonY());

        glm::mat4 view = player.getView();
        glm::mat4 projection = glm::perspective(glm::radians(player.getZoom()),
                                                static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
                                                0.1f,
                                                100.0f);


        //bottom.draw(view, projection);

         wall.draw(view, projection, {glm::vec3(0.0f)}, 1, glm::vec3(0.1f, 0.0f, 0.0f));

        // wall.draw(view, projection, wall_positions_0, 90, glm::vec3(1.0f, 0.0f, 0.0f));
        // wall.draw(view, projection, wall_positions_2, 90, glm::vec3(1.0f, 0.0f, 0.0f));
        //
        // wall.draw(view, projection, wall_positions_1, 0, glm::vec3(0.0f, 0.0f, 0.0f));
        // wall.draw(view, projection, wall_positions_3, 0, glm::vec3(0.0f, 0.0f, 0.0f));


        glfwSwapBuffers(window);
    }

    return 0;
}
