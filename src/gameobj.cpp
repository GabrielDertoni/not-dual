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

// Copy constructor.
GameObject::GameObject(const GameObject& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed)
{
    setTag(other.getTag());

    for (auto& [key, component] : other.components) {
        addComponentUniqueWithId(key, component->clone());
    }
}

GameObject::GameObject(GameObject&& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed)
{
    tag = std::move(other.tag);
    std::swap(components, other.components);
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

void GameObject::initialize(size_t self) {
    for (auto& [key, component] : components) {
        Behaviour *behaviour = dynamic_cast<Behaviour*>(component.get());
        if (behaviour) behaviour->initialize(*this);
    }
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

void GameObject::addComponentUniqueWithId(size_t id, std::unique_ptr<Component> component) {
    components.insert(std::make_pair(id, std::move(component)));
}

Component::Component() {}

/* Static stuff */

std::vector<GameObject> GameObject::instances;
std::deque<size_t> GameObject::destroyQueue;
std::deque<GameObject> GameObject::instantiateQueue;

void GameObject::markForDestruction(size_t idx) {
    destroyQueue.push_back(idx);
}

void GameObject::destroyAllMarked() {
    sort(destroyQueue.begin(), destroyQueue.end());
    while (!destroyQueue.empty()) {
        // Swap with last element and remove. O(1)
        std::swap(instances[destroyQueue.back()], *--instances.end());
        instances.pop_back();
        destroyQueue.pop_back();
    }
}

void GameObject::instantiateAllMarked() {
    while (!instantiateQueue.empty()) {
        instances.push_back(std::move(instantiateQueue.front()));
        instantiateQueue.pop_front();

        size_t i = instances.size() - 1;
        instances[i].initialize(i);
    }
}

void GameObject::addGameObject(GameObject gameObject) {
    instantiateQueue.push_back(std::move(gameObject));
}

std::vector<GameObject>& GameObject::getGameObjects() {
    return instances;
}

/* GameObjectBuilder */

GameObjectBuilder::GameObjectBuilder(Transform transform) :
    transform(transform)
{}

GameObjectBuilder& GameObjectBuilder::withTag(std::string&& tag) {
    this->tag = std::move(tag);
    return *this;
}

GameObject GameObjectBuilder::build() {
    GameObject obj = GameObject(transform);
    obj.tag = tag;

    for (auto& [key, component] : components) {
        obj.addComponentUniqueWithId(key, component->clone());
    }
    return obj;
}

void GameObjectBuilder::registerGameObject() {
    GameObject::addGameObject(build());
}
