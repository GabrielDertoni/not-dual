#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.hpp"

SpaceshipRenderer::SpaceshipRenderer(std::string texturePath) {
    texture.loadFromFile(texturePath);
    shape.setTexture(texture);
}

SpaceshipRenderer::SpaceshipRenderer(const SpaceshipRenderer& other) :
    texture(other.texture),
    shape(other.texture)
{}

void SpaceshipRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(shape, states);
}
