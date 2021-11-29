#include <iostream>
#include <chrono>
#include <ranges>

#include <chrono>
#include <ranges>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/superpower.hpp"
#include "includes/gameobj.hpp"
#include "includes/settings.hpp"
#include "includes/particles.hpp"

SuperPower::SuperPower(
    Timestamp lastPower
) :
    lastPower(lastPower)
{}

Spawner::Spawner(
    Timestamp created
) :
    lastPower(created)
{}

std::unique_ptr<Component> SuperPower::clone() {
    return std::make_unique<SuperPower>(*this);
}

void SuperPower::initialize(GameObject& gameObject) {}

void SuperPower::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - this->lastPower >= SUPER_POWER_INTERVAL_LIFE) {
        gameObject.destroy();
    }
}

Timestamp SuperPower::getLastPower() {
    return lastPower;
}

std::unique_ptr<Component> Spawner::clone() {
    return std::make_unique<Spawner>(*this);
}

void Spawner::initialize(GameObject& gameObject) {}

void Spawner::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - lastPower >= SUPER_POWER_INTERVAL) {
        GameObjectBuilder(gameObject.transform)
            .addComponent<SuperPower>(getNow())
            .addComponent<ParticleEmitter>()
            .addComponent<BoxCollider>(sf::Vector2f(POWER_SIZE, POWER_SIZE))
            .addComponent<Renderer>(RectangleShape(sf::Color::Red, sf::Vector2f(POWER_SIZE, POWER_SIZE)))
            .registerGameObject();

        lastPower = getNow();
    }
}

