#include <iostream>
#include <math.h>

#include "includes/gameobj.hpp"

Transform::Transform(sf::Vector2f position, float angle) :
    position(position),
    angle(angle)
{}

Transform::Transform(const Transform& other) :
    Transform(other.position, other.angle)
{}

sf::Transform Transform::getTranformMatrix() {
    sf::Transform t;
    t.rotate(angle).translate(position);
    return t;
}

GameObject::GameObject(Transform transform) :
    transform(transform),
    shouldBeDestroyed(false)
{}

GameObject::GameObject(const GameObject& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed)
{
    // TODO: Copy components.
    setTag(other.getTag());

    for (auto& [key, component] : other.components) {
        addComponentUnique(key, component->clone());
    }
}

GameObject::GameObject(const GameObject&& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed)
{
    tag = std::move(other.tag);

    for (auto& [key, component] : other.components) {
        components.insert(std::make_pair(key, component->clone()));
    }
}

GameObject& GameObject::operator=(GameObject &&other) {
    transform = other.transform;
    shouldBeDestroyed = other.shouldBeDestroyed;
    tag = std::move(other.tag);
    components = std::move(other.components);
    return *this;
}

void GameObject::destroy() {
    shouldBeDestroyed = true;
}

void GameObject::update(size_t self) {
    for (auto& [key, component] : components) {
        Behaviour *behaviour = dynamic_cast<Behaviour*>(component.get());
        if (behaviour) behaviour->update(*this);
    }

    if (shouldBeDestroyed) {
        markForDestruction(self);
    }
}

void GameObject::setTag(const std::string& tag) {
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

void GameObject::addComponentUnique(size_t id, std::unique_ptr<Component> component) {
    components.insert(std::make_pair(id, std::move(component)));
}

Component::Component() {}

/* Static stuff */

std::vector<GameObject> GameObject::instances;
std::deque<size_t> GameObject::destroyQueue;

void GameObject::markForDestruction(size_t idx) {
    destroyQueue.push_back(idx);
}

void GameObject::destroyAllMarked() {
    while (!destroyQueue.empty()) {
        // Swap with last element end remove. O(1)
        std::swap(instances[destroyQueue.front()], *--instances.end());
        instances.pop_back();
        destroyQueue.pop_front();
    }
}

void GameObject::addGameObject(GameObject gameObject) {
    instances.push_back(gameObject);
}

std::vector<GameObject>& GameObject::getGameObjects() {
    return instances;
}
