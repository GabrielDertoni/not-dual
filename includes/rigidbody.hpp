#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"
#include "includes/utils.hpp"

class RigidBody: public Behaviour {
public:
    // Mass, coefficient of friction and acceleation multiplier.
    //
    // `accMult` specifies how much the acceleration will be multiplied by in
    // every iteration it should be in range [0, 1].
    //
    // The `cof` is just a representation of that concept and has no intention
    // of beeing physically accurate. It is a value in range [0, 1] that is
    // multiplied by the velocity in every frame, thus, decresing it.
    //
    // `mass`    defaults to 1
    // `cof`     defaults to 1
    // `accMult` defaults to 0.5
    RigidBody(float mass, float cof, float accMult);
    RigidBody(float mass, float cof);
    RigidBody(float mass);
    RigidBody();
    RigidBody(const RigidBody& other);

    // Applies a force. Sets acceleration to vec/mass. On every iteration the
    // acceleration will be multiplied by `accMult`, a value in range [0, 1] and
    // thus nod increase over time.
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
