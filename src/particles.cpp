#include <iostream>
#include <math.h>
#include <chrono>

#include "includes/particles.hpp"
#include "includes/rigidbody.hpp"
#include "includes/renderer.hpp"
#include "includes/utils.hpp"

// TODO(#4): Parameterize those macros in the constructors.
#define N 10
#define PARTICLE_EMIT_INTERVAL (std::chrono::seconds(5))
#define PARTICLE_SIZE 5
#define PARTICLE_IMPULSE 2.0f
#define PARTICLE_TTL (std::chrono::milliseconds(500))
#define PARTICLE_VANISH_RATE 0.01f

void ParticleEmitter::initialize(GameObject& gameObject) {
    lastEmitted = getNow();
}

void ParticleEmitter::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - lastEmitted >= PARTICLE_EMIT_INTERVAL) {
        for (int i = 0; i < N; i++) {
            float ang = 2 * M_PI * (float)(rand() % 100) / 100;
            sf::Vector2f dir(cos(ang), sin(ang));
            GameObjectBuilder(gameObject.transform)
                .addComponent<Particle>(sf::Color::Red)
                .addComponentFrom([&] {
                    RigidBody rb(1.0f);
                    float impulse = PARTICLE_IMPULSE * (0.2 + (float)(rand() % 100) / 100.0);
                    rb.applyForce(dir * impulse);
                    return rb;
                })
                .addComponent<Renderer>(RectangleShape(sf::Vector2f(PARTICLE_SIZE, PARTICLE_SIZE)))
                .registerGameObject();
        }

        lastEmitted = getNow();
    }
}

Particle::Particle(sf::Color color) :
    color(color),
    lerp(0)
{}

Particle::Particle(const Particle& other) :
    color(other.color),
    lerp(other.lerp)
{}

void Particle::initialize(GameObject& gameObject) {
    created = getNow();
}

void Particle::update(GameObject& gameObject) {
    Timestamp now = getNow();
    if (now - created >= PARTICLE_TTL) {
        gameObject.destroy();
    }

    lerp = lerp + PARTICLE_VANISH_RATE > 1 ? 1 : lerp + PARTICLE_VANISH_RATE;
    sf::Color curr = colorLerp(color, sf::Color(255, 255, 255, 0), lerp);

    // TODO(#5): Make this more ergonomic.
    Renderer& renderer = gameObject.getComponent<Renderer>();
    RectangleShape* shape = dynamic_cast<RectangleShape*>(renderer.unsafeDrawablePtr());
    shape->setColor(curr);
}
