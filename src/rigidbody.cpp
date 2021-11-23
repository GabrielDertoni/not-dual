
#include "includes/rigidbody.hpp"

RigidBody::RigidBody(float mass) :
    mass(mass),
    velocity(0, 0),
    acceleration(0, 0)
{}

RigidBody::RigidBody(const RigidBody& other) :
    mass(other.mass),
    velocity(other.velocity),
    acceleration(other.acceleration)
{}

std::unique_ptr<Component> RigidBody::clone() {
    return std::make_unique<RigidBody>(*this);
}

void RigidBody::initialize(GameObject& gameObject) {}

void RigidBody::update(GameObject& gameObject) {
    gameObject.transform.position += velocity;
    velocity += acceleration;
    velocity *= DAMPENING;
    // acceleration = sf::Vector2f(0, 0);
    acceleration *= DAMPENING_ACC;
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
