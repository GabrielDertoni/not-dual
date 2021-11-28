#include <iostream>
#include <memory>
#include <array>
#include <ranges>
#include <math.h>

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
#include "includes/superpower.hpp"

static const sf::Vector2f size(PLAYER_SIZE, PLAYER_SIZE);

Player::Player(
    Controller* controller,
    Player::Side side,
    float life
) :
    life(life),
    side(side),
    controller(controller)
{
    lastShot = getNow();
}

Player::Player(
    Controller* controller,
    Player::Side side
) :
    Player(
       controller,
       side,
       100
    )
{}

Player::Player(const Player& other) :
    Player(other.controller, other.side, other.life)
{}

std::unique_ptr<Component> Player::clone() {
    return std::make_unique<Player>(*this);
}

void Player::initialize(GameObject& gameObject) {}

void Player::update(GameObject& gameObject) {
    auto inputs = controller->readInput();

    Timestamp now = getNow();
    auto ellapsed = now - lastShot;

    if (inputs[Controller::Shoot] && ellapsed > SHOOT_INTERVAL) {
        GameObjectBuilder(gameObject.transform)
            .withTag(std::string(gameObject.getTag()))
            .addComponent<Bullet>()
            .addComponent<BoxCollider>(sf::Vector2f(BULLET_SIZE, BULLET_SIZE))
            .addComponentFrom([&] {
                RigidBody rb(1.0f);
                sf::Vector2f dir;
                dir.x = side == LEFT ? 1 : -1;
                rb.setVelocity(dir * BULLET_SPEED);
                return rb;
            })
            .addComponent<Renderer>(RectangleShape(sf::Vector2f(BULLET_SIZE, BULLET_SIZE)))
            .registerGameObject();

        lastShot = getNow();
    }

    if (!gameObject.hasComponent<SuperPower>() /*&& ellapsed > SUPER_POWER_INTERVAL*/) {
        std::cout << "HERE" << std::endl;
        // this positions needs to be rand()
        auto superPowerPos = side == LEFT ? sf::Vector2f(250, HEIGHT / 2) :
                                            sf::Vector2f(450, HEIGHT / 2);
        GameObjectBuilder(Transform(superPowerPos, 0))
            .withTag(std::string(gameObject.getTag()))
            .addComponent<SuperPower>(getNow())
            .addComponent<BoxCollider>(sf::Vector2f(POWER_SIZE, POWER_SIZE))
            .addComponent<RigidBody>(1.0f)
            .addComponent<Renderer>(Spaceship(sf::Color::Red, POWER_SIZE))
            .registerGameObject();
    }

    BoxCollider& collider = gameObject.getComponent<BoxCollider>();

    sf::Vector2f vec(0, 0);

    if (inputs[Controller::Up   ]) vec.y += -1;
    if (inputs[Controller::Down ]) vec.y +=  1;
    if (inputs[Controller::Left ]) vec.x += -1;
    if (inputs[Controller::Right]) vec.x +=  1;

    float norm = sqrt(vec.x * vec.x + vec.y * vec.y);
    if (norm > 0) vec /= norm;
    vec *= IMPULSE;

    RigidBody& rb = gameObject.getComponent<RigidBody>();
    rb.applyForce(vec);

    sf::Vector2f zero(0, 0);
    std::array<sf::Vector2f, 4> normals;

    if (side == LEFT) {
       normals = {
            collider.gLeftTop.x <= 0             ? sf::Vector2f( 1,  0) : zero,
            collider.gLeftTop.y <= 0             ? sf::Vector2f( 0,  1) : zero,
            collider.gRightBottom.x >= WIDTH / 2 ? sf::Vector2f(-1,  0) : zero,
            collider.gRightBottom.y >= HEIGHT    ? sf::Vector2f( 0, -1) : zero,
        };
    } else {
       normals = {
            collider.gLeftTop.x <= WIDTH / 2  ? sf::Vector2f( 1,  0) : zero,
            collider.gLeftTop.y <= 0          ? sf::Vector2f( 0,  1) : zero,
            collider.gRightBottom.x >= WIDTH  ? sf::Vector2f(-1,  0) : zero,
            collider.gRightBottom.y >= HEIGHT ? sf::Vector2f( 0, -1) : zero,
        };
    }

    sf::Vector2f res = rb.velocity;
    for (sf::Vector2f normal : normals) {
        if (fabs(normal.x) > 0 || fabs(normal.y) > 0) {
            float normal_v_prod = normal.x * res.x + normal.y * res.y;
            if (normal_v_prod < 0) {
                float v_v_prod = res.x * res.x + res.y * res.y;
                float mag = sqrt(v_v_prod - normal_v_prod * normal_v_prod);
                float cross = normal.x * res.y - normal.y * res.x;

                sf::Vector2f perp;
                if (cross < 0) {
                    perp = sf::Vector2f(normal.y, -normal.x);
                } else {
                    perp = sf::Vector2f(-normal.y, normal.x);
                }

                res = perp * mag;
            }
        }
    }
    rb.velocity = res;

    auto isBulletHit = [&](GameObject& obj) {
        return obj.hasComponent<Bullet>()
            && obj.getTag() != gameObject.getTag()
            && collider.intersects(obj.getComponent<BoxCollider>());
    };

    auto playerGetsPower = [&](GameObject& obj) {
        return obj.hasComponent<SuperPower>()
            && obj.getTag() == gameObject.getTag()
            && collider.intersects(obj.getComponent<BoxCollider>());
    };

    for (auto& bullet : GameObject::getGameObjects()
                      | views::filter(isBulletHit))
    {
        life -= BULLET_DAMAGE;
        bullet.destroy();
    }

    for (auto& superPower : GameObject::getGameObjects()
                          | views::filter(playerGetsPower))
    {
       //do something with player ...
       superPower.destroy();
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
