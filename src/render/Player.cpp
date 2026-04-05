//
// Created by Tykhon Korol on 06.01.26.
//

#include "Player.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "../physics/CheckpointPhysics.h"
#include "../physics/raycast/RayCaster.h"


void Player::updateCameraVectors() {
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    new_front.y = sin(glm::radians(pitch));
    new_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(new_front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}

bool segmentIntersectsAABB(
    const glm::vec3 &A,
    const float radius,
    const glm::vec3 &B,
    const glm::vec3 &min,
    const glm::vec3 &max
) {
    glm::vec3 dir = B - A;

    float tmin = 0.0f;
    float tmax = 1.0f;

    for (int i = 0; i < 3; i++) {
        if (std::abs(dir[i]) < 1e-8f) {
            // движение параллельно плоскости
            if (A[i] < min[i] || A[i] > max[i])
                return false;
        } else {
            float ood = 1.0f / dir[i];
            float t1 = (min[i] - A[i]) * ood;
            float t2 = (max[i] - A[i]) * ood;

            if (t1 > t2) std::swap(t1, t2);

            tmin = std::max(tmin, t1);
            tmax = std::min(tmax, t2);

            if (tmin > tmax)
                return false;
        }
    }

    return true;
}

//TODO i need to make a dedicated class for collision checking, since it might be different objects and not only #WallPhysics
bool Player::updatePlayer(float floor_y, float delta_time, std::vector<WallPhysics> const &walls,
                          CheckpointPhysics &checkpoint) {
    glm::vec3 current_postion = position;
    glm::vec3 desired_position;
    float const velocity = movement_speed * delta_time;
    bool reached_checkpoint = false;
    for (auto const &move: moves) {
        switch (move) {
            case PlayerMovement::FORWARD: desired_position = current_postion + front * velocity;
                break;
            case PlayerMovement::BACKWARD: desired_position = current_postion - front * velocity;
                break;
            case PlayerMovement::RIGHT: desired_position = current_postion + right * velocity;
                break;
            case PlayerMovement::LEFT: desired_position = current_postion - right * velocity;
                break;
        }
        bool collision = false;
        for (auto const &wall: walls) {
            if (segmentIntersectsAABB(current_postion,
                                      radius,
                                      desired_position,
                                      wall.getAABB().min_vertex,
                                      wall.getAABB().max_vertex)) {
                collision = true;
                break;
            }
        }

        if (segmentIntersectsAABB(current_postion,
                                  radius,
                                  desired_position,
                                  checkpoint.getAABB().min_vertex,
                                  checkpoint.getAABB().max_vertex)) {
            checkpoint.triggerCollision();
            reached_checkpoint = true;
        }

        if (!collision) {
            current_postion = desired_position;
        }
    }
    position = current_postion;
    position.y = height + floor_y;
    moves.clear();
    return reached_checkpoint;
}


float Player::getZoom() {
    return zoom;
}

glm::vec3 Player::getPosition() {
    return position;
}

void Player::processCameraPositionMovement(PlayerMovement camera_movement) {
    switch (camera_movement) {
        case PlayerMovement::FORWARD:
            moves.push_back(PlayerMovement::FORWARD);
            break;
        case PlayerMovement::BACKWARD:
            moves.push_back(PlayerMovement::BACKWARD);
            break;
        case PlayerMovement::RIGHT:
            moves.push_back(PlayerMovement::RIGHT);
            break;
        case PlayerMovement::LEFT:
            moves.push_back(PlayerMovement::LEFT);
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

glm::vec3 rotateAroundAxis(const glm::vec3 &v, const glm::vec3 &axis, float angle) {
    // Rodrigues rotation formula
    return v * cos(angle) +
           glm::cross(axis, v) * sin(angle) +
           axis * glm::dot(axis, v) * (1.0f - cos(angle));
}

std::vector<Ray> Player::generateRayDirections() const {
    std::vector<Ray> rays;

    // Убираем pitch — только горизонталь
    glm::vec3 flat_front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

    for (int j = 0; j < n_rays_h; ++j) {
        float yaw_angle = -horizontal_fov * 0.5f +
                          j * (horizontal_fov / (n_rays_h - 1));

        glm::vec3 dir = rotateAroundAxis(flat_front, world_up, yaw_angle);

        rays.emplace_back(position, glm::normalize(dir));
    }

    return rays;
}

glm::vec3 Player::getFront() const {
    return front;
}
