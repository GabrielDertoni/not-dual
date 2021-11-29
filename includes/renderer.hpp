#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <memory>
#include <type_traits>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "includes/gameobj.hpp"
#include "includes/rendering.hpp"
#include "includes/utils.hpp"

class DrawableClonable: public sf::Drawable {
public:
    virtual std::unique_ptr<DrawableClonable> clone() const = 0;
};

class Renderer: public Component, public sf::Drawable {
public:
    template <typename T>
        requires std::is_base_of<DrawableClonable, T>::value
              && std::is_move_constructible<T>::value
    Renderer(T drawable) :
        drawable(std::make_unique<T>(std::move(drawable)))
    {}

    Renderer(const Renderer& other) : drawable(nullptr) {
        // TODO: Is this safe???
        drawable.reset(other.drawable->clone().release());
    }

    DERIVE_CLONE_COMPONENT

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(*drawable, states);
    }

    DrawableClonable* unsafeDrawablePtr() {
        return drawable.get();
    }

private:
    std::unique_ptr<DrawableClonable> drawable;
};

class RectangleShape: public DrawableClonable {
public:
    RectangleShape(sf::Color color, sf::Vector2f size) :
        rect(size)
    {
        rect.setFillColor(color);
    }

    RectangleShape(sf::Vector2f size) :
        RectangleShape(sf::Color::White, size)
    {}

    RectangleShape(const RectangleShape& other) :
        rect(other.rect)
    {}

    virtual std::unique_ptr<DrawableClonable> clone() const {
        return std::make_unique<RectangleShape>(*this);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(rect, states);
    }

    void setColor(sf::Color color) {
        rect.setFillColor(color);
    }

private:
    sf::RectangleShape rect;
};

#endif
