#include <chrono>
#include <ranges>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/superpower.hpp"
#include "includes/gameobj.hpp"
#include "includes/settings.hpp"
#include "includes/particles.hpp"

void SuperPower::initialize(GameObject& gameObject) {
    created = std::make_optional(getNow());
}

void SuperPower::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - *this->created >= SUPER_POWER_INTERVAL_LIFE) {
        gameObject.destroy();
    }
}

void PowerSpawner::initialize(GameObject& gameObject) {
    lastPower = std::make_optional(getNow());
}

void PowerSpawner::update(GameObject& gameObject) {
    Timestamp now = getNow();

    auto particleBuilder = GameObjectBuilder(Transform(sf::Vector2f(0, 0), 0));
    particleBuilder
        .addComponent<Particle>(sf::Color::Red, 2000, 0.01)
        .addComponent<RectangleRenderer>(sf::Vector2f(5, 5));

    if (now - *lastPower >= SUPER_POWER_INTERVAL) {
        GameObjectBuilder(gameObject.transform)
            .addComponent<SuperPower>()
            .addComponent<ParticleEmitter>(10, 100, 3.0f, particleBuilder)
            .addComponent<BoxCollider>(sf::Vector2f(POWER_SIZE, POWER_SIZE))
            .addComponent<RectangleRenderer>(sf::Color::Red, sf::Vector2f(POWER_SIZE, POWER_SIZE))
            .registerGameObject();

        lastPower = std::make_optional(getNow());
    }
}
