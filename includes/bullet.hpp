#ifndef _BULLET_H_
#define _BULLET_H_

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"

#define BULLET_SIZE 5

class Bullet: public Behaviour {
public:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
    virtual std::unique_ptr<Component> clone();

    Bullet(const Bullet& other);
    Bullet(BoxCollider container);

private:
    BoxCollider container;
};

#endif
