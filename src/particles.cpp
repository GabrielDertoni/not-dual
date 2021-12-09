#include <iostream>
#include <math.h>
#include <chrono>

#include "includes/particles.hpp"
#include "includes/rigidbody.hpp"
#include "includes/renderer.hpp"
#include "includes/utils.hpp"

// TODO(#4): Parameterize those macros in the constructors.
ParticleEmitter::ParticleEmitter(int numEmitAtOnce, int emitInterval,
                                 float avgParticleImpulse, GameObjectBuilder particleBuilder) :
    numEmitAtOnce(numEmitAtOnce),
    emitInterval(emitInterval),
    avgParticleImpulse(avgParticleImpulse),
    particleBuilder(particleBuilder)
{}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& other) :
    numEmitAtOnce(other.numEmitAtOnce),
    emitInterval(other.emitInterval),
    avgParticleImpulse(other.avgParticleImpulse),
    particleBuilder(other.particleBuilder)
{
    lastEmitted = other.lastEmitted;
}

void ParticleEmitter::initialize(GameObject& gameObject) {
    lastEmitted = getNow();
}

void ParticleEmitter::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - lastEmitted >= std::chrono::milliseconds(emitInterval)) {
        for (int i = 0; i < numEmitAtOnce; i++) {
            float ang = 2 * M_PI * (float)(rand() % 100) / 100;
            sf::Vector2f dir(cos(ang), sin(ang));
            particleBuilder
                .mapTransform([&](Transform transform) -> Transform {
                    return transform * gameObject.transform;
                })
                .addComponentFrom([&] {
                    RigidBody rb(1.0f);
                    float impulse = avgParticleImpulse * (0.2 + (float)(rand() % 100) / 100.0);
                    rb.applyForce(dir * impulse);
                    return rb;
                })
                .registerGameObject();
        }

        lastEmitted = getNow();
    }
}

Particle::Particle(sf::Color color, int ttl, float vanishRate) :
    color(color),
    ttl(ttl),
    vanishRate(vanishRate),
    lerp(0)
{}

Particle::Particle(const Particle& other) :
    color(other.color),
    ttl(other.ttl),
    vanishRate(other.vanishRate),
    lerp(other.lerp)
{}

void Particle::initialize(GameObject& gameObject) {
    created = getNow();
}

void Particle::update(GameObject& gameObject) {
    Timestamp now = getNow();
    if (now - created >= std::chrono::milliseconds(ttl)) {
        gameObject.destroy();
    }

    lerp = lerp + vanishRate > 1 ? 1 : lerp + vanishRate;
    sf::Color curr = colorLerp(color, sf::Color(255, 255, 255, 0), lerp);

    RectangleRenderer& renderer = gameObject.getComponent<RectangleRenderer>();
    renderer.setColor(curr);
}
