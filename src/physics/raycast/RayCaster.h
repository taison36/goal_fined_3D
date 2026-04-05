//
// Created by Tykhon Korol on 12.01.26.
//

#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>


class AABB;

struct Ray {
    glm::vec3 origin;
    glm::vec3 dir;

    Ray(const glm::vec3 &origin, const glm::vec3 &dir);
};

struct RayHit {
    glm::vec3 origin;
    glm::vec3 dir;
    float distance;

    RayHit(const glm::vec3 &origin, const glm::vec3 &dir, float distance)
        : origin(origin),
          dir(dir),
          distance(distance) {
    }
};

class RayCaster {
    [[nodiscard]] std::optional<float> rayCastDistance(Ray const &ray, AABB const &obstacle) const;

public:
    [[nodiscard]] std::optional<RayHit> cast(Ray const &ray, std::vector<AABB> const &obstacles) const;

    [[nodiscard]] std::optional<RayHit> cast(const glm::vec3 &origin_pos,
                                             const glm::vec3 &origin_front,
                                             const glm::vec3 &target_pos,
                                             const std::vector<AABB> &obstacles,
                                             float fovRadians) const;

    [[nodiscard]] std::vector<RayHit> castAll(std::vector<Ray> const &rays, std::vector<AABB> const &obstacles) const;
};

#endif //RAY_H
