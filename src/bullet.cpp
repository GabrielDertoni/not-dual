#include "includes/gameobj.hpp"
#include "includes/bullet.hpp"

static const sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);

Bullet::Bullet(
    Transform transform,
    sf::Vector2f vel,
    sf::Color color,
    BoxCollider collider,
    BoxCollider container
) :
    GameObject(transform),
    vel(vel),
    color(color),
    mesh(sf::Vector2f(BULLET_SIZE, BULLET_SIZE)),
    collider(collider),
    container(container)
{}

void Bullet::update() {
    transform.position += vel;

    sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);
    collider.leftTop = getPosition() - offset;
    collider.rightBottom = getPosition() + offset;

    if (collider.intersects(container)) {
        destroy();
    }

    mesh.setPosition(getPosition());
}

const sf::Drawable* Bullet::getMesh() const {
    return &mesh;
}

Bullet::Bullet(
    Transform transform,
    sf::Vector2f vel,
    sf::Color color,
    BoxCollider container
) :
    Bullet(
        transform,
        vel,
        color,
        BoxCollider(transform.position - offset, transform.position + offset),
        container
    )
{}

BoxCollider& Bullet::getCollider() {
    return collider;
}
