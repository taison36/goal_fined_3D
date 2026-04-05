//
// Created by Tykhon Korol on 06.01.26.
//

#ifndef PLAYER_H
#define PLAYER_H
#include <glm/glm.hpp>

#include "PlayerMovement.h"
#include "../physics/WallPhysics.h"

struct Ray;
constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 10.0f;
constexpr float SENSITIVITY = 0.1f;
constexpr float ZOOM = 45.0f;

constexpr float DEFAULT_HEIGHT = 1.8f;
constexpr float DEFAULT_RADIUS = 1.0f;

class CheckpointPhysics;

class Player {
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 world_up;

    const float height;
    const float radius;

    float yaw;
    float pitch;
    float zoom;

    const float camera_sensitivity;
    const float movement_speed;

    std::vector<PlayerMovement> moves;

    float horizontal_fov = glm::radians(90.0f);
    float vertical_fov = glm::radians(60.0f);

    int n_rays_h = 5; // число лучей по горизонтали
    int n_rays_v = 3; // число лучей по вертикали

    void updateCameraVectors();

public:
    bool updatePlayer(float floor_y, float delta_time, std::vector<WallPhysics> const &walls,
                      CheckpointPhysics &checkpoint);

    void processCameraPositionMovement(PlayerMovement camera_movement);

    void processMouseMovement(float xoffset, float yoffset);

    void processMouseScroll(float yoffset);

    glm::mat4 getView();

    float getZoom();

    glm::vec3 getPosition();

    [[nodiscard]] glm::vec3 getFront() const;

    [[nodiscard]] std::vector<Ray> generateRayDirections() const;

    explicit Player(const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 0.0f),
                    const glm::vec3 &up = glm::vec3(0.0f, 1.0f, 0.0f),
                    float yaw = YAW,
                    float pitch = PITCH,
                    float height = DEFAULT_HEIGHT,
                    float radius = DEFAULT_RADIUS
    );
};

#endif //PLAYER_H
