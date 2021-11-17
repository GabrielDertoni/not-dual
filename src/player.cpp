#include <math.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "includes/player.h"
#include "includes/globals.h"

Player::Player(sf::Vector2<float> pos, sf::Color color) :
    pos(pos),
    ang(0),
    vel(0, 0),
    acc(0, 0),
    color(color),
    mesh(pos, 0, color)
{}

void Player::update() {
    float impulse = 0.2;

    sf::Vector2f vec(0, 0);

    if (keysPressed[sf::Keyboard::W]) {
        vec += sf::Vector2f(0, -1);
    }

    if (keysPressed[sf::Keyboard::S]) {
        vec += sf::Vector2f(0, 1);
    }

    if (keysPressed[sf::Keyboard::D]) {
        vec += sf::Vector2f(1, 0);
    }

    if (keysPressed[sf::Keyboard::A]) {
        vec += sf::Vector2f(-1, 0);
    }

    float norm = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (norm > 0) vec /= norm;

    applyForce(vec * impulse);

    pos += vel;
    vel *= 0.99f;
    vel += acc;
    
    acc = sf::Vector2f(0, 0);

    mesh.pos = pos;
    mesh.ang = ang;
}

void Player::setRotation(float ang) {
    this->ang = ang;
}

void Player::applyForce(sf::Vector2f vec) {
    acc += vec;
}
