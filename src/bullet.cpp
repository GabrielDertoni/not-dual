#include "includes/gameobj.hpp"
#include "includes/bullet.hpp"

#define BULLET_SIZE 5

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
    mesh(sf::Vector2f(BULLET_SIZE, BULLET_SIZE)),
    collider(collider),
    container(container)
{}

void Bullet::update() {
    pos += vel;

    sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);
    collider.leftTop = pos - offset;
    collider.rightBottom = pos + offset;

    if (collider.intersects(container)) {
        destroy();
    }

    mesh.setPosition(pos);
}

sf::Drawable* Bullet::getMesh() {
    return &mesh;
}

Bullet Bullet::create(
    sf::Vector2f pos,
    sf::Vector2f vel,
    float ang,
    sf::Color color,
    BoxCollider container
) {
    sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);
    return Bullet(
        pos,
        vel,
        ang,
        color,
        BoxCollider(pos - offset, pos + offset),
        container
    );
}

std::unique_ptr<Bullet> Bullet::create_unique(
    sf::Vector2f pos,
    sf::Vector2f vel,
    sf::Color color,
    BoxCollider container
) {
    sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);
    return std::make_unique<Bullet>(
        pos,
        vel,
        0,
        color,
        BoxCollider(pos - offset, pos + offset),
        container
    );
}

BoxCollider& Bullet::getCollider() {
    return collider;
}
