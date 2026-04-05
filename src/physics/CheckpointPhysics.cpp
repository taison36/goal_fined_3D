#include "CheckpointPhysics.h"

#include <iostream>
#include <random>


void CheckpointPhysics::respawn() {
    static int i = 0;
    auto const &min = spawn_zone.min_vertex;
    auto const &max = spawn_zone.max_vertex;
    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_real_distribution<float> distX(min.x, max.x);
    std::uniform_real_distribution<float> distY(min.y, max.y);
    std::uniform_real_distribution<float> distZ(min.z, max.z);

    affine_transform.setPosition(glm::vec3(
            distX(gen),
            distY(gen),
            distZ(gen))
    );

    std::cout << "new coordinates for the checkpoint"
            << affine_transform.getPosition().x
            << " "
            << affine_transform.getPosition().y
            << " "
            << affine_transform.getPosition().z
            << " "
            << i++
            << std::endl;

    updateWorldAABB();
}

void CheckpointPhysics::updateWorldAABB() {
    world_aabb = local_aabb.convertByAffineTransform(affine_transform);
}

CheckpointPhysics::CheckpointPhysics(
    AffineTransform const &affine_transform,
    AABB const &model_local_aabb,
    AABB const &spawn_zone)
    : affine_transform(affine_transform),
      world_aabb(model_local_aabb.convertByAffineTransform(affine_transform)),
      local_aabb(model_local_aabb),
      spawn_zone(spawn_zone) {
}

CheckpointPhysics::CheckpointPhysics(
    AffineTransform &&affine_transform,
    AABB const &model_local_aabb,
    AABB const &spawn_zone)
    : affine_transform(affine_transform),
      world_aabb(model_local_aabb.convertByAffineTransform(affine_transform)),
      local_aabb(model_local_aabb),
      spawn_zone(spawn_zone) {
}

AABB CheckpointPhysics::getAABB() const {
    return world_aabb;
}

const AffineTransform &CheckpointPhysics::getAffineTransform() const {
    return affine_transform;
}

void CheckpointPhysics::triggerCollision() {
    respawn();
}
