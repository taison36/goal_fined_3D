//
// Created by Tykhon Korol on 06.01.26.
//

#include "Wall.h"

Wall::Wall(
    std::string const &model_path,
    std::string const &shader_vert_path,
    std::string const &shader_frag_path,
    float desired_size): model(Model(model_path)),
                         shader(Shader(shader_vert_path, shader_frag_path)),
                         desired_size(desired_size) {
}

void Wall::draw(glm::mat4 const &view_mat, glm::mat4 const &projection_mat, std::vector<glm::vec3> const& positions,
                float angle, glm::vec3 axis) {
    const float model_size = (model.min_vertex.x * -1) + model.max_vertex.x;
    const float scale_factor = 1.0f / (model_size / desired_size);

    shader.use();
    shader.setMat4("view", view_mat);
    shader.setMat4("projection", projection_mat);

    const float angle_radian = glm::radians(angle);

    for (const glm::vec3 &basePos: positions) {
        glm::mat4 model_mat(1.0f);
        model_mat = glm::translate(model_mat, basePos);
        model_mat = glm::rotate(model_mat, angle_radian, axis);
        model_mat = glm::scale(model_mat, glm::vec3(scale_factor));
        model_mat = glm::translate(model_mat, model.min_vertex);

        shader.setMat4("model", model_mat);
        model.Draw(shader);
    }
}
