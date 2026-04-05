//
// Created by Tykhon Korol on 12.01.26.
//

#include "RayCaster.h"

#include "../AABB.h"


Ray::Ray(const glm::vec3 &origin, const glm::vec3 &dir) : origin(origin), dir(dir) {
}

std::optional<float> RayCaster::rayCastDistance(Ray const &ray, AABB const &obstacle) const {
    auto const &min = obstacle.min_vertex;
    auto const &max = obstacle.max_vertex;

    float tmin = -std::numeric_limits<float>::infinity();
    float tmax = std::numeric_limits<float>::infinity();


    //checking intersection
    if (ray.dir.x != 0.0f) {
        float tx1 = (min.x - ray.origin.x) / ray.dir.x;
        float tx2 = (max.x - ray.origin.x) / ray.dir.x;
        tmin = std::max(tmin, std::min(tx1, tx2));
        tmax = std::min(tmax, std::max(tx1, tx2));
    } else if (ray.origin.x < min.x || ray.origin.x > max.x) {
        return std::nullopt;
    }

    if (ray.dir.y != 0.0f) {
        float ty1 = (min.y - ray.origin.y) / ray.dir.y;
        float ty2 = (max.y - ray.origin.y) / ray.dir.y;
        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));
    } else if (ray.origin.y < min.y || ray.origin.y > max.y) {
        return std::nullopt;
    }

    if (ray.dir.z != 0.0f) {
        float tz1 = (min.z - ray.origin.z) / ray.dir.z;
        float tz2 = (max.z - ray.origin.z) / ray.dir.z;
        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));
    } else if (ray.origin.z < min.z || ray.origin.z > max.z) {
        return std::nullopt;
    }

    if (tmax < 0.0f || tmin > tmax)
        return std::nullopt;

    return tmin; //tmin is the distance
}

std::optional<RayHit> RayCaster::cast(const Ray &ray, const std::vector<AABB> &obstacles) const {
    float min_dist = std::numeric_limits<float>::max();

    for (auto const &obstacle: obstacles) {
        if (auto found_distance = rayCastDistance(ray, obstacle)) {
            min_dist = std::min(found_distance.value(), min_dist);
        }
    }
    if (min_dist == std::numeric_limits<float>::max()) return std::nullopt;

    return RayHit(ray.origin, ray.dir, min_dist);
}

std::vector<RayHit> RayCaster::castAll(const std::vector<Ray> &rays, const std::vector<AABB> &obstacles) const {
    std::vector<RayHit> ray_hits;

    for (auto const &ray: rays) {
        if (auto hit = cast(ray, obstacles)) {
            ray_hits.push_back(hit.value());
        } else {
            ray_hits.emplace_back(ray.origin, ray.dir, -1);
        }
    }

    return ray_hits;
}

std::optional<RayHit> RayCaster::cast(const glm::vec3 &origin_pos,
                                      const glm::vec3 &origin_front,
                                      const glm::vec3 &target_pos,
                                      const std::vector<AABB> &obstacles,
                                      float fovRadians) const {
    const glm::vec3 to_target = target_pos - origin_pos;
    float dist_to_target = glm::length(to_target);
    const glm::vec3 dir_to_target = glm::normalize(to_target);

    // 1. FOV check
    float cosAngle = glm::dot(glm::normalize(origin_front), dir_to_target);
    if (cosAngle < std::cos(fovRadians * 0.5f))
        return std::nullopt;

    // 2. Raycast
    Ray ray(origin_pos, dir_to_target);

    for (const auto &obstacle: obstacles) {
        if (auto hitDist = rayCastDistance(ray, obstacle)) {
            if (*hitDist > 0.0f && *hitDist < dist_to_target) {
                //the wall is closer than the checkpoint
                return std::nullopt;
            }
        }
    }

    return RayHit(ray.origin, ray.dir, dist_to_target);
}
