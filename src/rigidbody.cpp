
#include "includes/rigidbody.hpp"

RigidBody::RigidBody(float mass, float cof, float accMult) :
    mass(mass),
    cof(cof),
    accMult(accMult),
    velocity(0, 0),
    acceleration(0, 0)
{}

RigidBody::RigidBody(float mass, float cof) :
    RigidBody(mass, cof, 0.5)
{}

RigidBody::RigidBody(float mass) :
    RigidBody(mass, 1)
{}

RigidBody::RigidBody(const RigidBody& other) :
    mass(other.mass),
    velocity(other.velocity),
    acceleration(other.acceleration),
    gravity(other.gravity)
{}

void RigidBody::initialize(GameObject& gameObject) {}

void RigidBody::update(GameObject& gameObject) {
    gameObject.transform.position += velocity;
    velocity += acceleration;

    // TODO(#7): Reintroduce dampening but parameterized in the constructor. Default could be 1.

    velocity *= cof;
    acceleration = sf::Vector2f(0, 0);

    // TODO(#6): Parameterize in constructor.
    acceleration *= accMult;

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

