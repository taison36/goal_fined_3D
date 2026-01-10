#include "AffineTransform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AffineTransform::AffineTransform(const glm::vec3 &position, const glm::vec3 &rotation, const glm::vec3 &scale)
    : position(position),
      rotation(rotation),
      scale(scale) {
}

glm::mat4 AffineTransform::getAffineTransformMatrix() const {
    glm::mat4 mat(1.0f);
    mat = glm::translate(mat, position);
    mat = glm::rotate(mat, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    mat = glm::rotate(mat, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    mat = glm::rotate(mat, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    mat = glm::scale(mat, scale);

    return mat;
}

void AffineTransform::setPosition(glm::vec3 const &new_pos) {
    position = new_pos;
}
