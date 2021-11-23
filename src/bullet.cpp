#include "includes/gameobj.hpp"
#include "includes/bullet.hpp"

static const sf::Vector2f offset(BULLET_SIZE, BULLET_SIZE);

Bullet::Bullet(const Bullet& other) :
    Bullet(other.container)
{}

Bullet::Bullet(BoxCollider container) :
    container(container)
{}

std::unique_ptr<Component> Bullet::clone() {
    return std::make_unique<Bullet>(*this);
}

void Bullet::initialize(GameObject& gameObject) {}

void Bullet::update(GameObject& gameObject) {
    BoxCollider& collider = gameObject.getComponent<BoxCollider>();

    if (collider.intersects(container)) {
        gameObject.destroy();
    }
}
