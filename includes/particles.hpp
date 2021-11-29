#ifndef _PARTICLES_H_
#define _PARTICLES_H_

#include <SFML/Graphics/Color.hpp>

#include "includes/gameobj.hpp"
#include "includes/time.hpp"

class Particle: public Behaviour {
public:
    Particle(sf::Color color);
    Particle(const Particle& other);

    virtual std::unique_ptr<Component> clone();

private:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);

    sf::Color color;
    float lerp;
    Timestamp created;
};

class ParticleEmitter: public Behaviour {
public:
    ParticleEmitter() = default;
    ParticleEmitter(const ParticleEmitter& other) = default;

    virtual std::unique_ptr<Component> clone();

private:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);

    Timestamp lastEmitted;
};

#endif
