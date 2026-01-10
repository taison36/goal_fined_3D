//
// Created by Tykhon Korol on 08.01.26.
//

#ifndef AFFINETRANSFORM_H
#define AFFINETRANSFORM_H

#include <glm/glm.hpp>

#include "../physics/AABB.h"

class AffineTransform{
    glm::vec3 position; // assuming that this is the center of the object
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    AffineTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale);

    [[nodiscard]] glm::vec3 getPosition() const {
        return position;
    }

    [[nodiscard]] glm::vec3 getRotation() const {
        return rotation;
    }

    [[nodiscard]] glm::vec3 getScale() const {
        return scale;
    }
    void setPosition(glm::vec3 const &new_pos);

    [[nodiscard]] glm::mat4 getAffineTransformMatrix() const;
};

#endif //AFFINETRANSFORM_H
