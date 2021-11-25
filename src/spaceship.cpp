#include <iostream>
#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "includes/rendering.hpp"
#include "includes/spaceship.hpp"

Spaceship::Spaceship(sf::Color color, float size) :
    size(size),
    shape(sf::Vector2f(size, size))
{
    auto half = sf::Vector2f(size / 2, size / 2);
    shape.setOrigin(half);
    shape.setFillColor(color);
}


Spaceship::Spaceship(const Spaceship& other) :
    size(other.size),
    shape(other.shape),
    matrix(other.matrix)
{
    printf("Copied\n");
    shape.setFillColor(other.shape.getFillColor());
}

void Spaceship::setTransformMatrix(sf::Transform matrix) {
    this->matrix = matrix;
    auto p = this->matrix.getMatrix();
    printf("[[%f %f %f %f]\n\
 [%f %f %f %f]\n\
 [%f %f %f %f]\n\
 [%f %f %f %f]]\n",
         p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10],
         p[11], p[12], p[13], p[14], p[15]);
}

void Spaceship::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    /*
        auto p = matrix.getMatrix();
        printf("[[%f %f %f %f]\n\
 [%f %f %f %f]\n\
 [%f %f %f %f]\n\
 [%f %f %f %f]]\n",
                 p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10],
                 p[11], p[12], p[13], p[14], p[15]);
                 */
    states.transform *= matrix;
    target.draw(shape, states);
}
