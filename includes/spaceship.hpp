#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/gameobj.hpp"
#include "includes/renderer.hpp"

class Spaceship: public DrawableClonable {
public:
    Spaceship(sf::Color color, float size);
    Spaceship(const Spaceship& other);

    virtual std::unique_ptr<DrawableClonable> clone() const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Color getColor() const;

private:
    float size;
    sf::RectangleShape shape;
};

#endif
