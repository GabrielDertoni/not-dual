#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/rendering.hpp"
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
    size(other.size),
    shape(other.shape)
{
    shape.setFillColor(other.shape.getFillColor());
}

std::unique_ptr<DrawableClonable> Spaceship::clone() const {
    return std::make_unique<Spaceship>(*this);
}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}

sf::Color Spaceship::getColor() const {
    return shape.getFillColor();
}
