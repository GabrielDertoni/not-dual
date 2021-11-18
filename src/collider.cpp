#include <algorithm>

#include <SFML/System/Vector2.hpp>

#include "includes/collider.h"


bool BoxCollider::intersects(BoxCollider& other) {
    if (!inverted) {
        if (!other.inverted) {
            float maxX = std::min(rightBottom.x, other.rightBottom.x);
            float minX = std::max(leftTop.x, other.leftTop.x);
            float maxY = std::min(rightBottom.y, other.leftTop.y);
            float minY = std::max(leftTop.y, other.leftTop.y);

            return minX <= maxX && minY <= maxY;
        } else {
            return !( leftTop.x     > other.leftTop.x
                   && leftTop.y     > other.leftTop.y
                   && rightBottom.x < other.rightBottom.x
                   && rightBottom.y < other.rightBottom.y
                    );
        }
    } else if (!other.inverted) {
        return  !( leftTop.x     < other.leftTop.x
                && leftTop.y     < other.leftTop.y
                && rightBottom.x > other.rightBottom.x
                && rightBottom.y > other.rightBottom.y
                );
    } else {
        // If they are both inverted, they surely intersect!
        return true;
    }

}
