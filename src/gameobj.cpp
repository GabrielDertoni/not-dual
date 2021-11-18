#include <iostream>
#include <math.h>

#include "includes/gameobj.hpp"
#include "includes/gamestate.hpp"

Transform::Transform(sf::Vector2f position, float angle) :
    position(position),
    angle(angle)
{}

Transform::Transform(const Transform& other) :
    Transform(other.position, other.angle)
{}

GameObject::GameObject(Transform transform) :
    transform(transform),
    shouldBeDestroyed(false)
{}

void GameObject::destroy() {
    shouldBeDestroyed = true;
}

void GameObject::callUpdate(std::list<std::unique_ptr<GameObject>>::iterator self) {
    update();

    if (shouldBeDestroyed) {
        markForDestruction(self);
    }
}

void GameObject::setTag(std::string tag) {
    this->tag = tag;
}

void GameObject::setPosition(sf::Vector2f position) {
    transform.position = position;
}

void GameObject::setRotation(float angle) {
    transform.angle = angle;
}

const std::string& GameObject::getTag() const {
    return tag;
}

sf::Vector2f GameObject::getPosition() const {
    return transform.position;
}

float GameObject::getRotation() const {
    return transform.angle;
}

float GameObject::getRotationRad() const {
    return M_PI * transform.angle / 180;
}

sf::Vector2f GameObject::getDir() const {
    float ang = getRotationRad();
    return sf::Vector2f(cos(ang), sin(ang));
}
