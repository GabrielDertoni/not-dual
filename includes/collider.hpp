#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"

class BoxCollider: public Component {
public:
    sf::Vector2f leftTop;
    sf::Vector2f rightBottom;

    BoxCollider(sf::Vector2f leftTop, sf::Vector2f rightBottom, bool inverted = false) :
        leftTop(leftTop),
        rightBottom(rightBottom),
        inverted(inverted)
    {}
    BoxCollider(const BoxCollider& other);

    bool intersects(BoxCollider& other);

    virtual std::unique_ptr<Component> clone();

private:
    bool inverted;
};

#endif
