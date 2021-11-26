#include <algorithm>

#include <SFML/System/Vector2.hpp>

#include "includes/collider.hpp"

BoxCollider::BoxCollider(const BoxCollider& other) :
    BoxCollider(other.leftTop, other.rightBottom)
{}

bool BoxCollider::intersects(BoxCollider& other) {
    float maxX = std::min(gRightBottom.x, other.gRightBottom.x);
    float minX = std::max(gLeftTop.x, other.gLeftTop.x);
    float maxY = std::min(gRightBottom.y, other.gLeftTop.y);
    float minY = std::max(gLeftTop.y, other.gLeftTop.y);

    return minX <= maxX && minY <= maxY;
}

std::unique_ptr<Component> BoxCollider::clone() {
    return std::make_unique<BoxCollider>(*this);
}

void BoxCollider::initialize(GameObject& gameObject) {
    sf::Transform matrix = gameObject.transform.getTranformMatrix();
    gLeftTop = matrix.transformPoint(leftTop);
    gRightBottom = matrix.transformPoint(rightBottom);
}

void BoxCollider::update(GameObject& gameObject) {
    sf::Transform matrix = gameObject.transform.getTranformMatrix();
    gLeftTop = matrix.transformPoint(leftTop);
    gRightBottom = matrix.transformPoint(rightBottom);
}


