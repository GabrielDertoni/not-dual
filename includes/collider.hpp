#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include <SFML/System/Vector2.hpp>

class BoxCollider {
public:
    BoxCollider(sf::Vector2f leftTop, sf::Vector2f rightBottom, bool inverted = false) :
        leftTop(leftTop),
        rightBottom(rightBottom),
        inverted(inverted)
    {}

    bool intersects(BoxCollider& other);

    sf::Vector2f leftTop;
    sf::Vector2f rightBottom;

private:
    bool inverted;
};

#endif
