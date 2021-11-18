#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.hpp"
Spaceship::Spaceship(GameObject* gameObject, sf::Color color, float size) :
    gameObject(gameObject),
    size(size),
    shape(sf::Vector2f(size, size))
{
    auto half = sf::Vector2f(size / 2, size / 2);
    shape.setOrigin(half);
    shape.setPosition(gameObject->getPosition());
    shape.setRotation(gameObject->getRotation());
    shape.setFillColor(color);
}

void Spaceship::update() {
    shape.setPosition(gameObject->getPosition());
    shape.setRotation(gameObject->getRotation());
}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape);
}
