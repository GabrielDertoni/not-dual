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

    if (now - *lastPower >= SUPER_POWER_INTERVAL) {
        GameObjectBuilder(gameObject.transform)
            .addComponent<SuperPower>()
            .addComponent<ParticleEmitter>(500, 100, 2.0f)
            .addComponent<BoxCollider>(sf::Vector2f(POWER_SIZE, POWER_SIZE))
            .addComponent<SpriteRenderer>(SB_PATH)
            .registerGameObject();

        lastPower = std::make_optional(getNow());
    }
}
