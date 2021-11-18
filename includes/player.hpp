#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <bitset>
#include <chrono>
#include <memory>
#include <math.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>

#include "includes/collider.hpp"
#include "includes/spaceship.hpp"
#include "includes/gameobj.hpp"

#define PLAYER_SIZE   20
#define IMPULSE       0.2f
#define DAMPENING     0.97f
#define DAMPENING_ACC 0.5f


class Controller {
public:
    enum Input {
        Up, Down, Left, Right, Shoot,
        NumInputs
    };

    virtual std::bitset<NumInputs> readInput() = 0;
};

class Player: public GameObject {
public:
    template <class Ctrl>
    static Player create(sf::Vector2f pos, float ang, sf::Color color, BoxCollider container);

    template <class Ctrl>
    static std::unique_ptr<Player> create_unique(sf::Vector2f pos, float ang, sf::Color color, BoxCollider container);

public:
    void setRotation(float ang);
    void applyForce(sf::Vector2f vec);
    bool isDead();

    virtual void update();
    virtual sf::Drawable* getMesh();

    Player(sf::Vector2f pos, float ang, sf::Color color,
           std::unique_ptr<Controller> controller, BoxCollider collider,
           BoxCollider container, float life);

private:
    sf::Vector2f pos;
    float ang;
    sf::Vector2f vel;
    sf::Vector2f acc;

    float life;

    sf::Color color;
    Spaceship mesh;
    BoxCollider collider;

    BoxCollider container;

    std::unique_ptr<Controller> controller;

    // std::chrono::time_point<std::chrono::high_resolution_clock> lastShot;
};

class WASDController: public Controller {
public:
    WASDController() {}
    virtual std::bitset<Controller::NumInputs> readInput();
};

class ArrowsController: public Controller {
public:
    ArrowsController() {}
    virtual std::bitset<Controller::NumInputs> readInput();
};


template <class Ctrl>
Player Player::create(sf::Vector2f pos, float ang, sf::Color color, BoxCollider container) {
    sf::Vector2f offset(PLAYER_SIZE, PLAYER_SIZE);
    return Player(
        pos,
        ang,
        color,
        std::make_unique<Ctrl>(),
        BoxCollider(pos - offset, pos + offset),
        container,
        100
    );
}

template <class Ctrl>
std::unique_ptr<Player> Player::create_unique(
    sf::Vector2f pos,
    float ang,
    sf::Color color,
    BoxCollider container
) {
    sf::Vector2f offset(PLAYER_SIZE, PLAYER_SIZE);
    return std::make_unique<Player>(
        pos,
        ang,
        color,
        std::make_unique<Ctrl>(),
        BoxCollider(pos - offset, pos + offset),
        container,
        100
    );
}


#endif
