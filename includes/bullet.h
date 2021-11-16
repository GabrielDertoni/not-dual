#ifndef _BULLET_H_
#define _BULLET_H_

#include "vec.h"

class Bullet {
public:
    Vec2<float> pos;
    float ang;
    Vec2<float> vel;
    Vec2<float> acc;

    Bullet(Vec2<float> pos, Vec2<float> vel) :
        pos(pos),
        ang(0),
        vel(vel),
        acc(0, 0) {}

    void update();
};

#endif
