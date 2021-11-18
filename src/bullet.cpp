#include "includes/bullet.hpp"

#define IMPULSE       0.2f
#define DAMPENING     0.97f
#define DAMPENING_ACC 0.5f

#define WIDTH  600
#define HEIGHT 400

Bullet::Bullet(
    sf::Vector2f pos,
    sf::Vector2f vel,
    float ang,
    sf::Color color,
    BoxCollider collider,
    BoxCollider container
) :
    pos(pos),
    vel(vel),
    ang(0),
    color(color),
    collider(collider),
    container(container)
{}

void Bullet::update() {
    pos += vel;

    sf::Vector2f offset(WIDTH, HEIGHT);
    collider.leftTop = pos - offset;
    collider.rightBottom = pos + offset;

    if (collider.intersects(container)) {
        vel = sf::Vector2f(0, 0);
    }

    mesh.setPosition(pos);
}

Bullet Bullet::create(
    sf::Vector2f pos,
    sf::Vector2f vel,
    float ang,
    sf::Color color,
    BoxCollider container) {
    sf::Vector2f offset(20, 20);
    return Bullet(
                pos,
                vel,
                ang,
                color,
                BoxCollider(pos - offset, pos + offset),
                container
            );
}

