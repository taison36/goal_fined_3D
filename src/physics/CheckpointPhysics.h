//
// Created by Tykhon Korol on 09.01.26.
//

#ifndef CHECKPOINTPHYSIC_H
#define CHECKPOINTPHYSIC_H

#include "../render/AffineTransform.h"
#include "AABB.h"
#include "IAABB.h"

class CheckpointPhysics : public IAABB {
    AffineTransform affine_transform;

    AABB world_aabb;

    AABB local_aabb;

    AABB spawn_zone;

    void respawn();

    void updateWorldAABB();

public:
    CheckpointPhysics(AffineTransform const &affine_transform,
                      AABB const &model_local_aabb,
                      AABB const &spawn_zone);

    CheckpointPhysics(AffineTransform &&affine_transform,
                      AABB const &model_local_aabb,
                      AABB const &spawn_zone);

    void triggerCollision();

    [[nodiscard]] AABB getAABB() const override;

    [[nodiscard]] const AffineTransform &getAffineTransform() const;
};

#endif //CHECKPOINTPHYSIC_H
