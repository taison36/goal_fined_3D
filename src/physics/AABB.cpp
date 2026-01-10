//
// Created by Tykhon Korol on 09.01.26.
//
#include "AABB.h"
#include "../render/AffineTransform.h"

AABB::AABB(glm::vec3 const &min_vertex, glm::vec3 const &max_vertex)
    : min_vertex(min_vertex),
      max_vertex(max_vertex) {
}

AABB AABB::operator*(float const scalar) const {
    return {
        min_vertex * scalar,
        max_vertex * scalar
    };
}

AABB AABB::operator*(glm::vec3 const &scalar) const {
    return {
        min_vertex * scalar,
        max_vertex * scalar
    };
}

AABB AABB::convertByAffineTransform(AffineTransform const &affine_transform) const {
    auto const &local_min = min_vertex;
    auto const &local_max = max_vertex;

    std::vector<glm::vec3> corners = {
        {local_min.x, local_min.y, local_min.z},
        {local_max.x, local_min.y, local_min.z},
        {local_min.x, local_max.y, local_min.z},
        {local_max.x, local_max.y, local_min.z},

        {local_min.x, local_min.y, local_max.z},
        {local_max.x, local_min.y, local_max.z},
        {local_min.x, local_max.y, local_max.z},
        {local_max.x, local_max.y, local_max.z}
    };

    auto const model_mat = affine_transform.getAffineTransformMatrix();

    for (auto &corner: corners) {
        glm::vec4 world = model_mat * glm::vec4(corner, 1.0f);
        corner = glm::vec3(world);
    }

    glm::vec3 new_min_vertex = corners[0];
    glm::vec3 new_max_vertex = corners[0];

    for (auto &corner: corners) {
        new_min_vertex = glm::min(new_min_vertex, corner);
        new_max_vertex = glm::max(new_max_vertex, corner);
    }

    return {
        new_min_vertex,
        new_max_vertex
    };
}
