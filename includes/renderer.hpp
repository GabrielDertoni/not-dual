#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <iostream>
#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "includes/gameobj.hpp"
#include "includes/rendering.hpp"

class Renderer: public Component, public sf::Drawable { };

class RectangleShape: public Renderer {
public:
    RectangleShape(sf::Vector2f size) :
        rect(size)
    {}

    RectangleShape(const RectangleShape& other) :
        rect(other.rect)
    {}

    virtual std::unique_ptr<Component> clone() {
        return std::make_unique<RectangleShape>(*this);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(rect, states);
    }

private:
    sf::RectangleShape rect;
};

#endif
