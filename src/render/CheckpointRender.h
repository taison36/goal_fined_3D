#ifndef CHECKPOINTRENDER_H
#define CHECKPOINTRENDER_H

#include "model/Model.h"
#include "../physics/AABB.h"
#include "../physics/IAABB.h"
#include "../physics/CheckpointPhysics.h"


class CheckpointRender : public IAABB {
    Model model;
    Shader shader;
    AABB local_aabb; // local abb, not scaled into the world space

    float static constexpr DEFAULT_SIZE = 8.1f;
    float const desired_size;

public:
    CheckpointRender(std::string const &model_path,
                     std::string const &shader_vert_path,
                     std::string const &shader_frag_path,
                     float desired_size = DEFAULT_SIZE);

    void draw(glm::mat4 const & view_mat,
              glm::mat4 const & projection_mat,
              CheckpointPhysics const &checkpoint_physicses);

    [[nodiscard]] AABB getAABB() const override;
};


#endif //CHECKPOINTRENDER_H
