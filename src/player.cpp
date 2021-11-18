#include <memory>
#include <math.h>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "includes/collider.hpp"
#include "includes/player.hpp"
#include "includes/input.hpp"

Player::Player(
    sf::Vector2<float> pos,
    sf::Color color,
    std::unique_ptr<Controller> controller,
    BoxCollider collider,
    BoxCollider container,
    float life
) :
    pos(pos),
    ang(0),
    vel(0, 0),
    acc(0, 0),
    life(life),
    color(color),
    mesh(pos, 0, color),
    collider(collider),
    container(container),
    controller(std::move(controller))
{}

void Player::update() {
    auto inputs = controller->readInput();
    sf::Vector2f vec(0, 0);

    if (inputs[Controller::Up   ]) vec.y += -1;
    if (inputs[Controller::Down ]) vec.y +=  1;
    if (inputs[Controller::Left ]) vec.x += -1;
    if (inputs[Controller::Right]) vec.x +=  1;

    float norm = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (norm > 0) vec /= norm;

    applyForce(vec * IMPULSE);

    sf::Vector2f prev = pos;

    pos += vel;
    vel *= DAMPENING;
    vel += acc;

    acc *= DAMPENING_ACC;

    sf::Vector2f offset(SIZE, SIZE);
    collider.leftTop = pos - offset;
    collider.rightBottom = pos + offset;

    if (collider.intersects(container)) {
        pos = prev;
        vel = sf::Vector2f(0, 0);
        acc = sf::Vector2f(0, 0);
    }

    mesh.pos = pos;
    mesh.ang = ang;
}

sf::Drawable* Player::getMesh() {
    return &mesh;
}

void Player::setRotation(float ang) {
    this->ang = ang;
}

void Player::applyForce(sf::Vector2f vec) {
    acc += vec;
}

bool Player::isDead() {
    return life <= 0;
}

std::bitset<Controller::NumInputs> WASDController::readInput() {
    std::bitset<Controller::NumInputs> set;
    set.reset();

    if (input::isKeyPressed(sf::Keyboard::W)) set.set(Controller::Up);
    if (input::isKeyPressed(sf::Keyboard::A)) set.set(Controller::Left);
    if (input::isKeyPressed(sf::Keyboard::S)) set.set(Controller::Down);
    if (input::isKeyPressed(sf::Keyboard::D)) set.set(Controller::Right);

    return set;
}

std::bitset<Controller::NumInputs> ArrowsController::readInput() {
    std::bitset<Controller::NumInputs> set;
    set.reset();

    if (input::isKeyPressed(sf::Keyboard::Up   )) set.set(Controller::Up);
    if (input::isKeyPressed(sf::Keyboard::Left )) set.set(Controller::Left);
    if (input::isKeyPressed(sf::Keyboard::Down )) set.set(Controller::Down);
    if (input::isKeyPressed(sf::Keyboard::Right)) set.set(Controller::Right);

    return set;
}
