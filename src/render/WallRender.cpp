//
// Created by Tykhon Korol on 06.01.26.
//

#include "WallRender.h"

WallRender::WallRender(
    std::string const &model_path,
    std::string const &shader_vert_path,
    std::string const &shader_frag_path,
    float desired_size): model(Model(model_path)),
                         shader(Shader(shader_vert_path, shader_frag_path)),
                         local_aabb(AABB(model.min_vertex, model.max_vertex)),
                         desired_size(desired_size) {
}

void WallRender::draw(glm::mat4 const &view_mat,
                      glm::mat4 const &projection_mat,
                      std::vector<WallPhysics> const &physics_walls) {
    shader.use();
    shader.setMat4("view", view_mat);
    shader.setMat4("projection", projection_mat);
    for (auto const &wall: physics_walls) {
        shader.setMat4("model", wall.getAffineTransform().getAffineTransformMatrix());
        model.Draw(shader);
    }
}

AABB WallRender::getAABB() const {
    return local_aabb;
}
