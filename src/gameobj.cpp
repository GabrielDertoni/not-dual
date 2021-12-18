#include <iostream>
#include <math.h>

#include "includes/gameobj.hpp"
#include "includes/uuid.hpp"

Transform::Transform(sf::Vector2f position, float angle) :
    position(position),
    angle(angle)
{}

Transform::Transform(const Transform& other) :
    Transform(other.position, other.angle)
{}

sf::Transform Transform::getTransformMatrix() const {
    sf::Transform t;
    t.rotate(angle).translate(position);
    return t;
}

Transform operator* (const Transform& lhs, const Transform& rhs) {
    // FIXME(#8): This is probably wrong.
    // Transform multiplication is actually a matrix multiplication, but when
    // the components are sperated like this, I don't know how to do it.
    // However, it should work for now.
    //
    // One possibility is to do something like
    //
    // ```cpp
    // sf::Transform t = lhs.getTransformMatrix() * rhs.getTransformMatrix();
    // sf::Vector2f origin = t.transformPoint(sf::Vector2f(0.0, 0.0));
    // sf::Vector2f down   = t.transformPoint(sf::Vector2f(1.0, 0.0));
    // // Do something with `down` in order to figure out the angle and `origin`
    // // is the position.
    // ```
    return Transform(rhs.position - lhs.position, rhs.angle - lhs.angle);
}

GameObject::GameObject(Transform transform) :
    transform(transform),
    shouldBeDestroyed(false)
{
    uuid = uuid::generate_uuid_v4();
}

// Copy constructor.
GameObject::GameObject(const GameObject& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed.load())
{
    setTag(other.getTag());
    uuid = uuid::generate_uuid_v4();

    for (auto& component : other.components) {
        components.push_back(component->clone());
    }
}

GameObject::GameObject(GameObject&& other) :
    transform(other.transform),
    shouldBeDestroyed(other.shouldBeDestroyed.load())
{
    tag = std::move(other.tag);
    uuid = std::move(other.uuid);
    std::swap(components, other.components);
}

GameObject& GameObject::operator=(GameObject &&other) {
    transform = other.transform;
    shouldBeDestroyed = other.shouldBeDestroyed.load();
    tag = std::move(other.tag);
    uuid = std::move(other.uuid);
    components = std::move(other.components);
    return *this;
}

void GameObject::destroy() const {
    ((GameObject*)this)->shouldBeDestroyed = true;
}

void GameObject::initialize() {
    for (auto& component : components) {
        Behaviour *behaviour = dynamic_cast<Behaviour*>(component.get());
        if (behaviour) behaviour->initialize(*this);
    }
}

void GameObject::update() {
    for (auto& component : components) {
        Behaviour *behaviour = dynamic_cast<Behaviour*>(component.get());
        if (behaviour) behaviour->update(*this);
    }

    if (shouldBeDestroyed) {
        markForDestruction(uuid);
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

const std::string& GameObject::getUUID() const {
    return uuid;
}

Component::Component() {}

/* Static stuff */

std::map<std::string, GameObjectHandle> GameObject::instances;
std::mutex GameObject::destroyQueueLock;
std::deque<std::string> GameObject::destroyQueue;
std::mutex GameObject::instantiateQueueLock;
std::deque<std::shared_ptr<GameObject>> GameObject::instantiateQueue;

void GameObject::destroyAllInstances(){
    instances.clear();
}

void GameObject::markForDestruction(std::string id) {
    std::unique_lock lock(destroyQueueLock);
    destroyQueue.push_back(id);
}

void GameObject::destroyAllMarked() {
    while (!destroyQueue.empty()) {
        std::string& id = destroyQueue.front();
        auto it = instances.find(id);
        if (it == instances.end()) {
            throw std::runtime_error("unexpected object to destroy is already destroyed");
        }
        instances.erase(it);
        destroyQueue.pop_front();
    }
}

void GameObject::instantiateAllMarked() {
    while (!instantiateQueue.empty()) {
        std::string uuid = instantiateQueue.front()->getUUID();
        auto [it, ok] = instances.emplace(uuid, std::move(instantiateQueue.front()));

        if (!ok) {
            throw std::runtime_error("unable to insert GameObject, UUID already present");
        }
        instantiateQueue.pop_front();
        it->second.acquire()->initialize();
    }
}

void GameObject::addGameObject(GameObject gameObject) {
    std::unique_lock lock(instantiateQueueLock);
    instantiateQueue.push_back(std::make_shared<GameObject>(std::move(gameObject)));
}

void GameObject::addComponentUnique(std::unique_ptr<Component> component) {
    components.push_back(std::move(component));
}

std::map<std::string, GameObjectHandle>& GameObject::getGameObjects() {
    return instances;
}

/* GameObjectBuilder */

GameObjectBuilder::GameObjectBuilder(Transform transform) :
    transform(transform)
{}

GameObjectBuilder::GameObjectBuilder(const GameObjectBuilder& other) :
    transform(other.transform)
{
    tag = other.tag;
    for (auto& component : components) {
        addComponentUnique(component->clone());
    }
}

GameObjectBuilder& GameObjectBuilder::withTag(std::string&& tag) {
    this->tag = std::move(tag);
    return *this;
}

GameObject GameObjectBuilder::build() {
    GameObject obj = GameObject(transform);
    obj.tag = tag;

    for (auto& component : components) {
        obj.addComponentUnique(component->clone());
    }
    return obj;
}

void GameObjectBuilder::registerGameObject() {
    GameObject::addGameObject(build());
}

/* GameObjectHandle */

GameObjectHandle::GameObjectHandle(std::shared_ptr<GameObject> gameObject) :
    gameObject(gameObject)
{}

GameObjectHandle::GameObjectHandle(GameObject&& gameObject) :
    GameObjectHandle(std::make_unique<GameObject>(std::move(gameObject)))
{}

GameObjectWriteGuard GameObjectHandle::acquire() {
    std::unique_lock<std::mutex> guard(sharedPtrLock);
    // Only use the copy constructor once locked.
    std::shared_ptr<GameObject> ptr = gameObject;
    return GameObjectWriteGuard(std::unique_lock<std::mutex>(writeLock), std::move(ptr));
}

GameObjectGuard GameObjectHandle::operator->() {
    std::unique_lock<std::mutex> guard(sharedPtrLock);
    // Only use the copy constructor once locked.
    std::shared_ptr<GameObject> ptr = gameObject;
    return GameObjectGuard(std::move(gameObject));
}

/* GameObjectWriteGuard */

GameObjectWriteGuard::GameObjectWriteGuard(std::unique_lock<std::mutex>&& guard, std::shared_ptr<GameObject>&& gameObject) :
    guard(std::move(guard)),
    gameObject(gameObject)
{}

GameObjectWriteGuard::~GameObjectWriteGuard() {
    gameObject = nullptr;
    guard.unlock();
}

GameObject* GameObjectWriteGuard::operator->() {
    // This is ok because we know the mutex is locked
    return gameObject.get();
}

/* GameObjectGuard */

GameObjectGuard::GameObjectGuard(std::shared_ptr<GameObject>&& gameObject) :
    gameObject(gameObject)
{}

const GameObject* GameObjectGuard::operator->() {
    // This is ok because we only give out an immutable pointer.
    return gameObject.get();
}
