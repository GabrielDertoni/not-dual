#ifndef _BULLET_H_
#define _BULLET_H_

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"

#define BULLET_SIZE 5

class Bullet: public GameObject {
public:
    virtual void update();
    virtual const sf::Drawable* getMesh() const;

    Bullet(Transform transform, sf::Vector2f vel, sf::Color color,
           BoxCollider container);
    Bullet(Transform transform, sf::Vector2f vel, sf::Color color,
           BoxCollider collider, BoxCollider container);

    BoxCollider& getCollider();

private:
    sf::Vector2f vel;

    sf::Color color;
    sf::RectangleShape mesh;
    BoxCollider collider;

    BoxCollider container;

};

#endif
