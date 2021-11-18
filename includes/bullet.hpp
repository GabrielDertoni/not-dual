#ifndef _BULLET_H_
#define _BULLET_H_

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "includes/spaceship.hpp"
#include "includes/collider.hpp"

class Bullet: public GameObject {
public:
    static Bullet create(sf::Vector2f pos, sf::Vector2f vel, float ang,
            sf::Color color, BoxCollider container);

    static std::unique_ptr<Bullet> create_unique(sf::Vector2f pos, sf::Vector2f vel,
            sf::Color color, BoxCollider container);

    virtual void update();
    virtual sf::Drawable* getMesh();

    Bullet(sf::Vector2f pos, sf::Vector2f vel, float ang, sf::Color color,
           BoxCollider collider, BoxCollider container);

    BoxCollider& getCollider();

private:
    sf::Vector2f pos;
    sf::Vector2f vel;
    float ang;

    sf::Color color;
    sf::RectangleShape mesh;
    BoxCollider collider;

    BoxCollider container;

};

#endif
