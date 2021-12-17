#include <iostream>
#include <math.h>
#include <chrono>

#include "includes/particles.hpp"
#include "includes/rigidbody.hpp"
#include "includes/renderer.hpp"
#include "includes/settings.hpp"
#include "includes/utils.hpp"

ParticleEmitter::ParticleEmitter(int numEmitAtOnce, int emitInterval,
                                 float avgParticleImpulse) :
    numEmitAtOnce(numEmitAtOnce),
    emitInterval(emitInterval),
    avgParticleImpulse(avgParticleImpulse)
{}

ParticleEmitter::ParticleEmitter(const ParticleEmitter& other) :
    numEmitAtOnce(other.numEmitAtOnce),
    emitInterval(other.emitInterval),
    avgParticleImpulse(other.avgParticleImpulse)
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
            Transform transform = gameObject.transform;
            transform.position += sf::Vector2f(
                5 * ((float)(rand() % 100) / 50.0 - 1.0),
                5 * ((float)(rand() % 100) / 50.0 - 1.0)
            );
            GameObjectBuilder(transform)
                .addComponent<Particle>(sf::Color::Red, 500, 0.01)
                .addComponent<SpriteRenderer>(P_PATH)
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
    vanishRate(vanishRate)
{}

Particle::Particle(const Particle& other) :
    color(other.color),
    ttl(other.ttl),
    vanishRate(other.vanishRate)
{}

void Particle::initialize(GameObject& gameObject) {
    created = getNow();
}

void Particle::update(GameObject& gameObject) {
    Timestamp now = getNow();
    if (now - created >= std::chrono::milliseconds(ttl)) {
        gameObject.destroy();
    }
}
