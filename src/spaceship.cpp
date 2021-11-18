#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.hpp"
#include "includes/player.hpp"

Spaceship::Spaceship(const Spaceship& other) {}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::CircleShape shape(PLAYER_SIZE, 4);
    shape.setFillColor(color);
    shape.setOrigin(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    shape.setPosition(pos);
    shape.setRotation(ang + 45);
    target.draw(shape);
}
