//
// Created by Tykhon Korol on 06.01.26.
//

#ifndef BOTTOM_H
#define BOTTOM_H
#include "model/Model.h"

constexpr float DEFAULT_SIZE = 8.0f;

class Bottom {
    Model model;
    Shader shader;

    std::vector<glm::vec3> positions;

    const float common_y;
    const float desired_size;

public:
    Bottom(std::vector<glm::vec3> const &positions,
           std::string const &model_path,
           std::string const &shader_vert_path,
           std::string const &shader_frag_path,
           float desired_size = DEFAULT_SIZE,
           float common_y = 0.0f);

    void draw(glm::mat4 const &view_mat, glm::mat4 const &projection_mat);

    float getCommonY();
};
#endif //BOTTOM_H
