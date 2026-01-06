//
// Created by Tykhon Korol on 06.01.26.
//
#include "Bottom.h"

Bottom::Bottom(std::vector<glm::vec3> const &positions,
               std::string const &model_path,
               std::string const &shader_vert_path,
               std::string const &shader_frag_path,
               float desired_size,
               float common_y)
    : model(Model(model_path)),
      shader(Shader(shader_vert_path, shader_frag_path)),
      positions(positions),
      common_y(common_y),
      desired_size(desired_size) {
}

float Bottom::getCommonY() {
    return common_y;
}


void Bottom::draw(glm::mat4 const &view_mat, glm::mat4 const &projection_mat) {
    const float model_size = (model.min_vertex.x * -1) + model.max_vertex.x;
    const float scale_factor = 1.0f / (model_size / desired_size);

    shader.use();
    shader.setMat4("view", view_mat);
    shader.setMat4("projection", projection_mat);

    float angle = glm::radians(90.0f);
    glm::vec3 axis(1.0f, 0.0f, 0.0f);
    auto min = model.min_vertex * scale_factor;
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle, axis);
    glm::vec4 rotated = rotMat * glm::vec4(min, 0.0f);
    min = glm::vec3(rotated);

    for (const glm::vec3 &basePos: positions) {
        auto pos = basePos + min;

        glm::mat4 model_mat(1.0f);
        model_mat = glm::translate(model_mat, pos);
        model_mat = glm::rotate(model_mat, angle, axis);
        model_mat = glm::scale(model_mat, glm::vec3(scale_factor));

        shader.setMat4("model", model_mat);
        model.Draw(shader);
    }
}
