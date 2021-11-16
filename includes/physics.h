#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "vec.h"

namespace Physics {
    class Transform {
        Vec2<float> pos;
        float ang;
    };

    class RigidBody {
        Vec2<float> vel;
        Vec2<float> acc;
    };
}

#endif
