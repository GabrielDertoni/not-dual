#include <iostream>
#include <memory>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "includes/collider.hpp"
#include "includes/gamestate.hpp"
#include "includes/player.hpp"
#include "includes/bullet.hpp"
#include "includes/input.hpp"

static const sf::Vector2f size(PLAYER_SIZE, PLAYER_SIZE);

Player::Player(
    Transform transform,
    sf::Color color,
    Controller* controller,
    BoxCollider collider,
    BoxCollider container,
    float life
) :
    GameObject(transform),
    vel(0, 0),
    acc(0, 0),
    life(life),
    color(color),
    mesh(this, color, PLAYER_SIZE),
    collider(collider),
    container(container),
    controller(controller)
{
    lastShot = getNow();
}

Player::Player(
    Transform transform,
    sf::Color color,
    Controller* controller,
    BoxCollider container
) :
    Player(
       transform,
       color,
       controller,
       BoxCollider(transform.position - size, transform.position + size),
       container,
       100
    )
{}

void Player::update() {
    auto inputs = controller->readInput();
    sf::Vector2f vec(0, 0);

    if (inputs[Controller::Up   ]) vec.y += -1;
    if (inputs[Controller::Down ]) vec.y +=  1;
    if (inputs[Controller::Left ]) vec.x += -1;
    if (inputs[Controller::Right]) vec.x +=  1;

    Timestamp now = getNow();
    auto ellapsed = now - lastShot;

    if (inputs[Controller::Shoot] && ellapsed > SHOOT_INTERVAL) {
        sf::Vector2f vec = getDir() * BULLET_SPEED;

        auto bullet = std::make_unique<Bullet>(
            transform,
            vec,
            sf::Color::White,
            worldCollider
        );
        bullet->setTag(tag);
        addGameObject(std::move(bullet));

        lastShot = getNow();
    }

    float norm = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (norm > 0) vec /= norm;

    applyForce(vec * IMPULSE);

    sf::Vector2f prev = getPosition();

    transform.position += vel;
    vel *= DAMPENING;
    vel += acc;
    acc *= DAMPENING_ACC;

    sf::Vector2f offset(PLAYER_SIZE, PLAYER_SIZE);
    collider.leftTop = getPosition() - offset;
    collider.rightBottom = getPosition() + offset;

    if (collider.intersects(container)) {
        setPosition(prev);
        vel = sf::Vector2f(0, 0);
        acc = sf::Vector2f(0, 0);
    }

    for (auto& obj : gameObjects) {
        Bullet* bullet = dynamic_cast<Bullet*>(obj.get());
        if (bullet != nullptr && collider.intersects(bullet->getCollider()) &&
            bullet->getTag() != tag) {
            life -= BULLET_DAMAGE;
            bullet->destroy();
        }
    }

    if (life <= 0) {
        destroy();
    }

    mesh.update();
}

const sf::Drawable* Player::getMesh() const {
    return &mesh;
}

void Player::applyForce(sf::Vector2f vec) {
    acc += vec;
}

bool Player::isDead() {
    return life <= 0;
}

WASDController wasdController;
ArrowsController arrowsController;

std::bitset<Controller::NumInputs> WASDController::readInput() {
    std::bitset<Controller::NumInputs> set;
    set.reset();

    if (input::isKeyPressed(sf::Keyboard::W       )) set.set(Controller::Up);
    if (input::isKeyPressed(sf::Keyboard::A       )) set.set(Controller::Left);
    if (input::isKeyPressed(sf::Keyboard::S       )) set.set(Controller::Down);
    if (input::isKeyPressed(sf::Keyboard::D       )) set.set(Controller::Right);
    if (input::isKeyPressed(sf::Keyboard::LControl)) set.set(Controller::Shoot);

    return set;
}

std::bitset<Controller::NumInputs> ArrowsController::readInput() {
    std::bitset<Controller::NumInputs> set;
    set.reset();

    if (input::isKeyPressed(sf::Keyboard::Up      )) set.set(Controller::Up);
    if (input::isKeyPressed(sf::Keyboard::Left    )) set.set(Controller::Left);
    if (input::isKeyPressed(sf::Keyboard::Down    )) set.set(Controller::Down);
    if (input::isKeyPressed(sf::Keyboard::Right   )) set.set(Controller::Right);
    if (input::isKeyPressed(sf::Keyboard::RControl)) set.set(Controller::Shoot);

    return set;
}
