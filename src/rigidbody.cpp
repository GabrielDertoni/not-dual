
#include "includes/rigidbody.hpp"

RigidBody::RigidBody(float mass) :
    mass(mass),
    velocity(0, 0),
    acceleration(0, 0)
{}

RigidBody::RigidBody(const RigidBody& other) :
    mass(other.mass),
    velocity(other.velocity),
    acceleration(other.acceleration),
    gravity(other.gravity)
{}

std::unique_ptr<Component> RigidBody::clone() {
    return std::make_unique<RigidBody>(*this);
}

void RigidBody::initialize(GameObject& gameObject) {}

void RigidBody::update(GameObject& gameObject) {
    gameObject.transform.position += velocity;
    velocity += acceleration;

    // TODO: Reintroduce dampening but parameterized in the constructor. Default
    // could be 1.

    // velocity *= DAMPENING;
    // acceleration = sf::Vector2f(0, 0);

    // TODO: Parameterize in constructor.
    acceleration *= DAMPENING_ACC;

    applyForce(gravity);
}

void RigidBody::applyForce(sf::Vector2f vec) {
    acceleration = vec / mass;
}

void RigidBody::setVelocity(sf::Vector2f vec) {
    velocity = vec;
}

void RigidBody::setAcceleration(sf::Vector2f vec) {
    acceleration = vec;
}

void RigidBody::setGravity(sf::Vector2f vec) {
    gravity = vec;
}

