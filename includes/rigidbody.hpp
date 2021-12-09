#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"
#include "includes/utils.hpp"

class RigidBody: public Behaviour {
public:
    // Mass, coefficient of friction and acceleation multiplier. `accMult`
    // specifies how much the acceleration will be multiplied by in every
    // iteration.
    RigidBody(float mass, float cof, float accMult);
    RigidBody(float mass, float cof);
    RigidBody(float mass);
    RigidBody(const RigidBody& other);

    void applyForce(sf::Vector2f vec);
    void setVelocity(sf::Vector2f vec);
    void setAcceleration(sf::Vector2f vec);
    void setGravity(sf::Vector2f vec);

    DERIVE_CLONE_COMPONENT

    float mass;
    float cof;
    float accMult;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Vector2f gravity;

private:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
