//
// Created by Tykhon Korol on 08.01.26.
//

#ifndef IPOSITIONABLE_H
#define IPOSITIONABLE_H

#include "AABB.h"

class IAABB {
public:
    [[nodiscard]] virtual AABB getAABB() const = 0;

    virtual ~IAABB() = default;
};

#endif //IPOSITIONABLE_H
