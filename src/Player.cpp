//
// Created by Tykhon Korol on 06.01.26.
//

#include "Player.h"
#include <glm/gtc/matrix_transform.hpp>

void Player::updateCameraVectors() {
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

void Player::updatePlayer(float floor_y) {
    float bottom = position.y - height * 0.5f;
    if (bottom < floor_y) {
        position.y = floor_y + height * 0.5f;
    }
}

float Player::getZoom() {
    return zoom;
}

glm::vec3 Player::getPosition() {
    return position;
}

void Player::processCameraPositionMovement(PlayerMovement camera_movement, float delta_time) {
    float velocity = movement_speed * delta_time;
    switch (camera_movement) {
        case FORWARD:
            position += front * velocity;
            break;
        case BACKWARD:
            position -= front * velocity;
            break;
        case RIGHT:
            position += right * velocity;
            break;
        case LEFT:
            position -= right * velocity;
            break;
    }
}

void Player::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= camera_sensitivity;
    yoffset *= camera_sensitivity;
    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    } else if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    updateCameraVectors();
}

void Player::processMouseScroll(float yoffset) {
    zoom -= yoffset;
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

glm::mat4 Player::getView() {
    return glm::lookAt(position, position + front, up);
}

Player::Player(const glm::vec3 &position,
               const glm::vec3 &up,
               float yaw,
               float pitch,
               float height,
               float radius
) : position(position),
    front(glm::vec3(0.0f, 0.0f, -1.0f)),
    up(up),
    world_up(up),
    height(height),
    radius(radius),
    yaw(yaw),
    pitch(pitch),
    zoom(ZOOM),
    camera_sensitivity(SENSITIVITY),
    movement_speed(SPEED) {

    updateCameraVectors();
}
