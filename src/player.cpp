#include <iostream>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "../includes/player.h"

void Player::update() {
    
}


void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    float size = 40;
    sf::CircleShape shape(size, 3);
    shape.setFillColor(color);
    shape.setOrigin(sf::Vector2f(size, size));
    shape.setPosition(pos);
    shape.setRotation(ang);
    target.draw(shape);
}

void Player::setRotation(float ang) {
    this->ang = ang;
}
