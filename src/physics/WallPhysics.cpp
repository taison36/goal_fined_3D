//
// Created by Tykhon Korol on 08.01.26.
//

#include "WallPhysics.h"

WallPhysics::WallPhysics(
    AffineTransform const &affine_transform,
    AABB model_local_aabb
): affine_transform(affine_transform),
   aabb(model_local_aabb.convertByAffineTransform(affine_transform)) {
}

WallPhysics::WallPhysics(
    AffineTransform &&affine_transform,
    AABB model_local_aabb
): affine_transform(affine_transform),
   aabb(model_local_aabb.convertByAffineTransform(affine_transform)) {
}

AABB WallPhysics::getAABB() const {
    return aabb;
}

const AffineTransform &WallPhysics::getAffineTransform() const {
    return affine_transform;
}
