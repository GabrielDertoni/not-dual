#include "includes/gameobj.hpp"
#include "includes/bullet.hpp"
#include "includes/settings.hpp"

Bullet::Bullet(bool isSuper) : isSuper(isSuper) { }
Bullet::Bullet(const Bullet& other) : isSuper(other.isSuper) { }

std::unique_ptr<Component> Bullet::clone() {
    return std::make_unique<Bullet>(*this);
}

void Bullet::initialize(GameObject& gameObject) {}

void Bullet::update(GameObject& gameObject) {
    Transform& transform = gameObject.transform;
    if (transform.position.x > WIDTH || transform.position.x < 0
     || transform.position.y > HEIGHT || transform.position.y < 0)
    {
        gameObject.destroy();
    }
}
