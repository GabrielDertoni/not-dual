#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <bitset>
#include <math.h>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/collider.h"
#include "includes/spaceship.h"
#include "includes/gameobj.h"

#define SIZE          20
#define IMPULSE       0.2f
#define DAMPENING     0.97f
#define DAMPENING_ACC 0.5f


class Controller {
public:
    enum Input {
        Up, Down, Left, Right, NumInputs
    };

    virtual std::bitset<NumInputs> readInput() = 0;
};

class Player: public GameObject {
public:
    template <class Ctrl>
    static Player create(sf::Vector2f pos, sf::Color color, BoxCollider container);

public:
    void setRotation(float ang);
    void applyForce(sf::Vector2f vec);

    virtual void update();
    virtual sf::Drawable* getMesh();

private:
    sf::Vector2f pos;
    float ang;
    sf::Vector2f vel;
    sf::Vector2f acc;

    sf::Color color;
    Spaceship mesh;
    BoxCollider collider;

    BoxCollider container;

    std::unique_ptr<Controller> controller;

    Player(sf::Vector2f pos, sf::Color color,
           std::unique_ptr<Controller> controller, BoxCollider collider,
           BoxCollider container);
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
Player Player::create(sf::Vector2f pos, sf::Color color, BoxCollider container) {
    sf::Vector2f offset(SIZE, SIZE);
    return Player(
                pos,
                color,
                std::make_unique<Ctrl>(),
                BoxCollider(pos - offset, pos + offset),
                container
            );
}


#endif
