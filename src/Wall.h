//
// Created by Tykhon Korol on 06.01.26.
//

#ifndef WALL_H
#define WALL_H
#include "util/Model.h"

class Wall {
    Model model;
    Shader shader;

    static constexpr float DEFAULT_SIZE = 8.1f;

    const float desired_size;

public:
    Wall(std::string const &model_path,
         std::string const &shader_vert_path,
         std::string const &shader_frag_path,
         float desired_size = DEFAULT_SIZE);

    void draw(glm::mat4 const &view_mat, glm::mat4 const &projection_mat, std::vector<glm::vec3> const& positions,
                    float angle, glm::vec3 axis);
};

#endif //WALL_H
