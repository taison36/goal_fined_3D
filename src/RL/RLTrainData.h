//
// Created by Tykhon Korol on 13.01.26.
//

#ifndef RLDATA_H
#define RLDATA_H
#include <optional>
#include <vector>

#include <glm/glm.hpp>

struct RayHit;

struct RlRay {
    float distance;
    bool visible;

    RlRay() = default;

    RlRay(float distance, bool visible)
        : distance(distance),
          visible(visible)
          {
    }
};

struct RLTrainData {
    std::vector<RlRay> rays;
    RlRay checkpoint_hit;
    bool reached_checkpoint;
    glm::vec3 checkpoint_dir;

    RLTrainData(std::optional<RayHit> const &checkpoint_hit, std::vector<RayHit> const &vision_rays,
                bool reached_checkpoint);

    [[nodiscard]] std::string getSerialized() const;

    [[nodiscard]] std::vector<RlRay> getRays() const {
        return rays;
    }

    [[nodiscard]] RlRay getCheckpointHit() const {
        return checkpoint_hit;
    }
};


#endif //RLDATA_H
