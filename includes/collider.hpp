#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"
#include "includes/utils.hpp"

class BoxCollider: public Behaviour {
public:
    sf::Vector2f leftTop;
    sf::Vector2f rightBottom;

    sf::Vector2f gLeftTop;
    sf::Vector2f gRightBottom;

    BoxCollider(sf::Vector2f leftTop, sf::Vector2f rightBottom) :
        leftTop(leftTop),
        rightBottom(rightBottom)
    {}
    BoxCollider(sf::Vector2f size) :
        leftTop(-size/2.0f),
        rightBottom(size/2.0f)
    {}
    BoxCollider(const BoxCollider& other);

    bool intersects(BoxCollider& other) const;

    DERIVE_CLONE_COMPONENT

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
