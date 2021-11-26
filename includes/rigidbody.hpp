#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"

#define DAMPENING     0.97f
#define DAMPENING_ACC 0.5f

class RigidBody: public Behaviour {
public:
    RigidBody(float mass);
    RigidBody(const RigidBody& other);

    void applyForce(sf::Vector2f vec);
    void setVelocity(sf::Vector2f vec);
    void setAcceleration(sf::Vector2f vec);

    virtual std::unique_ptr<Component> clone();

    float mass;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;

private:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
