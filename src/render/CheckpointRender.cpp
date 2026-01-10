//
// Created by Tykhon Korol on 09.01.26.
//

#include "CheckpointRender.h"

CheckpointRender::CheckpointRender(
    std::string const &model_path,
    std::string const &shader_vert_path,
    std::string const &shader_frag_path,
    float desired_size): model(Model(model_path)),
                         shader(Shader(shader_vert_path, shader_frag_path)),
                         local_aabb(AABB(model.min_vertex, model.max_vertex)),
                         desired_size(desired_size) {
}

void CheckpointRender::draw(glm::mat4 const &view_mat, glm::mat4 const &projection_mat,
                            CheckpointPhysics const &checkpoint_physicses) {
    shader.use();
    shader.setMat4("view", view_mat);
    shader.setMat4("projection", projection_mat);

    shader.setMat4("model", checkpoint_physicses.getAffineTransform().getAffineTransformMatrix());
    model.Draw(shader);
}

AABB CheckpointRender::getAABB() const {
    return local_aabb;
}
