#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/rendering.hpp"
#include "includes/spaceship.hpp"

SpaceshipRenderer::SpaceshipRenderer(sf::Color color, float size) :
    size(size),
    shape(sf::Vector2f(size, size))
{
    auto half = sf::Vector2f(size / 2, size / 2);
    shape.setOrigin(half);
    shape.setFillColor(color);
}


SpaceshipRenderer::SpaceshipRenderer(const SpaceshipRenderer& other) :
    size(other.size),
    shape(other.shape)
{
    shape.setFillColor(other.shape.getFillColor());
}

sf::Color SpaceshipRenderer::getColor() const {
    return shape.getFillColor();
}

void SpaceshipRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}
