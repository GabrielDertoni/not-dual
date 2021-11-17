#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/System/Vector2.hpp>

#include "includes/spaceship.h"
#include "includes/gameobj.h"

class Player: public GameObject {
public:
    sf::Vector2f pos;
    float ang;
    sf::Vector2f vel;
    sf::Vector2f acc;

    sf::Color color;
    Spaceship mesh;

    Player(sf::Vector2f pos, sf::Color color);

    void setRotation(float ang);
    void applyForce(sf::Vector2f vec);

    virtual void update();
};

#endif
