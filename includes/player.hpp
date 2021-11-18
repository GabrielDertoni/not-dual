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
#include "includes/time.hpp"

#define PLAYER_SIZE 20
// How much force is applied when the player presses a button.
#define IMPULSE       0.2f
#define DAMPENING     0.97f
#define DAMPENING_ACC 0.5f

#define BULLET_SPEED  5.0f
#define BULLET_DAMAGE 1
#define SHOOT_INTERVAL (std::chrono::milliseconds(500))

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
    void setRotation(float ang);
    void applyForce(sf::Vector2f vec);
    bool isDead();

    virtual void update();
    virtual const sf::Drawable* getMesh() const;

    Player(Transform transform, sf::Color color,
           Controller* controller, BoxCollider container);
    Player(Transform transform, sf::Color color,
           Controller* controller, BoxCollider collider,
           BoxCollider container, float life);

private:
    sf::Vector2f vel;
    sf::Vector2f acc;

    float life;

    sf::Color color;
    Spaceship mesh;
    BoxCollider collider;

    BoxCollider container;

    Controller* controller;

    Timestamp lastShot;
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

extern WASDController wasdController;
extern ArrowsController arrowsController;


#endif
