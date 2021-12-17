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
#include "includes/gameobj.hpp"
#include "includes/time.hpp"
#include "includes/utils.hpp"

#define PLAYER_SIZE   32
// How much force is applied when the player presses a button.
#define IMPULSE       0.2f

#define BULLET_SPEED  5.0f
#define BULLET_DAMAGE 10
#define SUPER_BULLET_DAMAGE 30
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

    enum Side {
        LEFT, RIGHT
    };

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);

    DERIVE_CLONE_COMPONENT

    Player(Controller* controller, Side side);
    Player(Controller* controller, Side side, float life);
    Player(const Player& other);

private:
    float life;
    Side side;
    Controller* controller;

    Timestamp lastShot;
    bool hasPower;
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
