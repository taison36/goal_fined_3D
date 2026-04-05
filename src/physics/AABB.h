#ifndef AABB_H
#define AABB_H

#include <glm/vec3.hpp>

class AffineTransform;

class AABB {
public:
    glm::vec3 min_vertex;
    glm::vec3 max_vertex;

    AABB(const glm::vec3 &min_vertex, const glm::vec3 &max_vertex);

    AABB operator*(float scalar) const;

    AABB operator*(glm::vec3 const &scalar) const;


    [[nodiscard]] AABB convertByAffineTransform(AffineTransform const &affine_transform) const;
};

#endif //AABB_H
