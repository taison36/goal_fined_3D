//
// Created by Tykhon Korol on 06.01.26.
//

#ifndef WALL_H
#define WALL_H
#include "../physics/WallPhysics.h"
#include "model/Model.h"

class WallRender : public IAABB {
    Model model;
    Shader shader;
    AABB local_aabb; // local abb, not scaled into the world space

    static constexpr float DEFAULT_SIZE = 8.1f;

    const float desired_size;

public:
    WallRender(std::string const &model_path,
               std::string const &shader_vert_path,
               std::string const &shader_frag_path,
               float desired_size = DEFAULT_SIZE);

    void draw(glm::mat4 const &view_mat,
              glm::mat4 const &projection_mat,
              std::vector<WallPhysics> const &physics_walls);

    [[nodiscard]] AABB getAABB() const override;
};

#endif //WALL_H
