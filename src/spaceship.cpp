#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.hpp"
Spaceship::Spaceship(sf::Color color, float size) :
    size(size),
    shape(sf::Vector2f(size, size))
{
    auto half = sf::Vector2f(size / 2, size / 2);
    shape.setOrigin(half);
    shape.setFillColor(color);
}


Spaceship::Spaceship(const Spaceship& other) :
    Spaceship(sf::Color::White, other.size)
{
    shape.setFillColor(other.shape.getFillColor());
}

std::unique_ptr<Component> Spaceship::clone() {
    return std::make_unique<Spaceship>(*this);
}

void Spaceship::initialize(GameObject& gameObject) {
    shape.setPosition(gameObject.getPosition());
    shape.setRotation(gameObject.getRotation());
}

void Spaceship::update(GameObject& gameObject) {
    shape.setPosition(gameObject.getPosition());
    shape.setRotation(gameObject.getRotation());
}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape);
}
