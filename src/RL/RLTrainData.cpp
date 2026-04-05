//
// Created by Tykhon Korol on 13.01.26.
//

#include "RLTrainData.h"

#include "../physics/raycast/RayCaster.h"
#include <nlohmann/json.hpp>


RLTrainData::RLTrainData(std::optional<RayHit> const &checkpoint_ray_hit,
                         std::vector<RayHit> const &vision_rays, bool reached_checkpoint): reached_checkpoint(
    reached_checkpoint) {
    RlRay checkpoint_rlray{};
    if (const auto hit = checkpoint_ray_hit) {
        checkpoint_rlray.visible = true;
        checkpoint_rlray.distance = hit.value().distance;
        checkpoint_dir = hit.value().dir;
    } else {
        checkpoint_rlray.visible = false;
        checkpoint_rlray.distance = 0;
    }

    std::vector<RlRay> vision_rlrays;

    for (auto const &hit: vision_rays) {
        if (hit.distance != -1) {
            vision_rlrays.emplace_back(hit.distance, true);
        } else {
            vision_rlrays.emplace_back(-1, false);
        }
    }

    checkpoint_hit = checkpoint_rlray;
    rays = vision_rlrays;
}

using json = nlohmann::json;

// RlRay

void to_json(json &j, const RlRay &r) {
    j = json{
        {"distance", r.distance},
        {"visible", r.visible},
    };
}

void from_json(const json &j, RlRay &r) {
    j.at("distance").get_to(r.distance);
    j.at("visible").get_to(r.visible);
}

// RLData

void to_json(json &j, const RLTrainData &d) {
    j = json{
        {"rays", d.rays},
        {"checkpoint_hit", d.checkpoint_hit},
        {"reached_checkpoint", d.reached_checkpoint},
        {"cp_dir_x", d.checkpoint_dir.x},
        {"cp_dir_z", d.checkpoint_dir.z}
    };
}

void from_json(const json &j, RLTrainData &d) {
    j.at("rays").get_to(d.rays);
    j.at("checkpoint_hit").get_to(d.checkpoint_hit);
    j.at("reached_checkpoint").get_to(d.reached_checkpoint);
}

std::string RLTrainData::getSerialized() const {
    const json j = *this;
    return j.dump();
}
