#ifndef _BULLET_H_
#define _BULLET_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "../includes/spaceship.h"

#include "../includes/collider.h"

class Bullet {
public:
    static Bullet create(sf::Vector2f pos, sf::Vector2f vel, float ang,
            sf::Color color, BoxCollider container);

    virtual void update();

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float ang;

    sf::Color color;
    sf::RectangleShape mesh;
    BoxCollider collider;

    BoxCollider container;

    Bullet(sf::Vector2f pos, sf::Vector2f vel, float ang, sf::Color color,
           BoxCollider collider, BoxCollider container);

};

#endif
