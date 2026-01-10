//
// Created by Tykhon Korol on 08.01.26.
//

#ifndef WALLPHYSICS_H
#define WALLPHYSICS_H
#include "AABB.h"
#include "../render/AffineTransform.h"
#include "IAABB.h"


class WallPhysics : public IAABB {
    AffineTransform affine_transform;

    AABB aabb;

public:
    WallPhysics(AffineTransform const &affine_transform, AABB model_local_aabb);

    WallPhysics(AffineTransform &&affine_transform, AABB model_local_aabb);

    [[nodiscard]] AABB getAABB() const override;

    [[nodiscard]] const AffineTransform &getAffineTransform() const;
};


#endif //WALLPHYSICS_H
