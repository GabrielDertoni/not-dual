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
#include "includes/particles.hpp"

#define N_PARTICLES 50
#define PARTICLE_SIZE 5
#define PARTICLE_IMPULSE 2.0f
#define PARTICLE_TTL (std::chrono::milliseconds(1000))
#define PARTICLE_VANISH_RATE 0.05f


static const sf::Vector2f size(PLAYER_SIZE, PLAYER_SIZE);

Player::Player(
    Controller* controller,
    Player::Side side,
    float life
) :
    life(life),
    side(side),
    controller(controller),
    hasPower(false)
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
{
    hasPower = other.hasPower;
}

std::unique_ptr<Component> Player::clone() {
    return std::make_unique<Player>(*this);
}

void Player::initialize(GameObject& gameObject) {}

void Player::update(GameObject& gameObject) {
    auto inputs = controller->readInput();

    Timestamp now = getNow();
    auto ellapsed = now - lastShot;

    if (inputs[Controller::Shoot] && ellapsed > SHOOT_INTERVAL) {
        auto builder = GameObjectBuilder(gameObject.transform);
        builder
            .withTag(std::string(gameObject.getTag()))
            .addComponent<Bullet>(hasPower)
            .addComponentFrom([&] {
                RigidBody rb(1.0f);
                sf::Vector2f dir;
                dir.x = side == LEFT ? 1 : -1;
                rb.setVelocity(dir * BULLET_SPEED);
                return rb;
            });


        if (hasPower) {
            builder
                .addComponent<BoxCollider>(sf::Vector2f(SUPER_BULLET_SIZE, SUPER_BULLET_SIZE))
                .addComponent<Renderer>(RectangleShape(sf::Vector2f(SUPER_BULLET_SIZE, SUPER_BULLET_SIZE)));
        } else {
            builder
                .addComponent<BoxCollider>(sf::Vector2f(BULLET_SIZE, BULLET_SIZE))
                .addComponent<Renderer>(RectangleShape(sf::Vector2f(BULLET_SIZE, BULLET_SIZE)));
        }

        builder.registerGameObject();

        lastShot = getNow();
        if (hasPower) hasPower = false;
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
            && collider.intersects(obj.getComponent<BoxCollider>());
    };

    for (auto& bullet : GameObject::getGameObjects()
                      | views::filter(isBulletHit))
    {
        Bullet& behaviour = bullet.getComponent<Bullet>();
        life -= behaviour.isSuper ? SUPER_BULLET_DAMAGE : BULLET_DAMAGE;
        bullet.destroy();

        for (int i = 0; i < N_PARTICLES; i++) {
            float ang = 2 * M_PI * (float)(rand() % 100) / 100;
            sf::Vector2f dir(cos(ang), sin(ang));
            GameObjectBuilder(gameObject.transform)
                .addComponent<Particle>(sf::Color::Red)
                .addComponentFrom([&] {
                    RigidBody rb(1.0f);
                    float impulse = PARTICLE_IMPULSE * (0.2 + (float)(rand() % 100) / 100.0);
                    rb.applyForce(dir * impulse);
                    rb.setGravity(sf::Vector2f(0, 0.5f));
                    return rb;
                })
                .addComponent<Renderer>(RectangleShape(sf::Vector2f(PARTICLE_SIZE, PARTICLE_SIZE)))
                .registerGameObject();
        }
    }

    for (auto& superPower : GameObject::getGameObjects()
                          | views::filter(playerGetsPower))
    {
        hasPower = true;
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
