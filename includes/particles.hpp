#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include <SFML/Graphics/Color.hpp>

#include "includes/gameobj.hpp"
#include "includes/time.hpp"
#include "includes/utils.hpp"

class Particle: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    // ttl is Time to Live (measured in milliseconds)
    Particle(sf::Color color, int ttl, float vanishRate);
    Particle(const Particle& other);

private:
    sf::Color color;
    int ttl;
    float vanishRate;
    Timestamp created;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

class ParticleEmitter: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    ParticleEmitter(int numEmitAtOnce, int emitInterval, float avgParticleImpulse);
    ParticleEmitter(const ParticleEmitter& other);

private:
    int numEmitAtOnce;
    int emitInterval;
    float avgParticleImpulse;
    Timestamp lastEmitted;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
