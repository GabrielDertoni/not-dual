#ifndef _BULLET_H_
#define _BULLET_H_

#include <SFML/System/Vector2.hpp>

class Bullet {
public:
    Vector2f pos;
    float ang;
    Vector2f vel;
    Vector2f acc;

    BoxCollider collider;

    Bullet(Vector2f pos, Vector2f vel, BoxCollider collider) :
        pos(pos),
        ang(0),
        vel(vel),
        acc(0, 0),
        collider(collider)
    {}

    void update();
};

#endif
