#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.h"
#include "includes/player.h"

Spaceship::Spaceship(const Spaceship& other) {}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::CircleShape shape(SIZE, 4);
    shape.setFillColor(color);
    shape.setOrigin(sf::Vector2f(SIZE, SIZE));
    shape.setPosition(pos);
    shape.setRotation(ang);
    target.draw(shape);
}
