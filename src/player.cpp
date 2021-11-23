#include <iostream>
#include <memory>
#include <ranges>

namespace views = std::views;

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "includes/collider.hpp"
#include "includes/player.hpp"
#include "includes/rigidbody.hpp"
#include "includes/bullet.hpp"
#include "includes/input.hpp"
#include "includes/settings.hpp"

static const sf::Vector2f size(PLAYER_SIZE, PLAYER_SIZE);

static const BoxCollider worldCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, HEIGHT), true);

Player::Player(
    Controller* controller,
    BoxCollider container,
    float life
) :
    life(life),
    container(container),
    controller(controller)
{
    lastShot = getNow();
}

Player::Player(
    Controller* controller,
    BoxCollider container
) :
    Player(
       controller,
       container,
       100
    )
{}

Player::Player(const Player& other) :
    Player(other.controller, other.container, other.life)
{}

std::unique_ptr<Component> Player::clone() {
    return std::make_unique<Player>(*this);
}

void Player::initialize(GameObject& gameObject) {}

void Player::update(GameObject& gameObject) {
    auto inputs = controller->readInput();
    sf::Vector2f vec(0, 0);

    if (inputs[Controller::Up   ]) vec.y += -1;
    if (inputs[Controller::Down ]) vec.y +=  1;
    if (inputs[Controller::Left ]) vec.x += -1;
    if (inputs[Controller::Right]) vec.x +=  1;

    Timestamp now = getNow();
    auto ellapsed = now - lastShot;

    if (inputs[Controller::Shoot] && ellapsed > SHOOT_INTERVAL) {
        GameObject bullet(gameObject.transform);
        bullet.setTag(gameObject.getTag());
        bullet.addComponent<Bullet>(worldCollider);
        RigidBody& rb = bullet.addComponent<RigidBody>(1.0f);
        rb.setVelocity(gameObject.getDir() * BULLET_SPEED);

        GameObject::addGameObject(std::move(bullet));

        lastShot = getNow();
    }

    float norm = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (norm > 0) vec /= norm;

    RigidBody& rb = gameObject.getComponent<RigidBody>();
    rb.applyForce(vec * IMPULSE);

    BoxCollider& collider = gameObject.getComponent<BoxCollider>();

    if (collider.intersects(container)) {
        // TODO
    }

    auto isBulletHit = [&](GameObject& obj) {
        return obj.hasComponent<Bullet>()
            && obj.getTag() != gameObject.getTag()
            && collider.intersects(obj.getComponent<BoxCollider>());
    };

    for (auto bullet : GameObject::getGameObjects() | views::filter(isBulletHit)) {
        life -= BULLET_DAMAGE;
        bullet.destroy();
    }

    if (life <= 0) {
        gameObject.destroy();
    }
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
