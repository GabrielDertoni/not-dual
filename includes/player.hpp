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

#define PLAYER_SIZE   20
// How much force is applied when the player presses a button.
#define IMPULSE       0.2f

#define BULLET_SPEED  5.0f
#define BULLET_DAMAGE 10
#define SHOOT_INTERVAL (std::chrono::milliseconds(100))

class Controller {
public:
    enum Input {
        Up, Down, Left, Right, Shoot,
        NumInputs
    };

    virtual std::bitset<NumInputs> readInput() = 0;
};

class Player: public Behaviour {
public:
    bool isDead();

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
    virtual std::unique_ptr<Component> clone();

    Player(Controller* controller, BoxCollider container);
    Player(Controller* controller, BoxCollider container, float life);
    Player(const Player& other);

private:
    float life;
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
