#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <memory>
#include <type_traits>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include "includes/gameobj.hpp"
#include "includes/utils.hpp"

class Renderer: public Component, public sf::Drawable {
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class RectangleRenderer: public Renderer {
public:
    DERIVE_CLONE_COMPONENT

    RectangleRenderer(sf::Color color, sf::Vector2f size);
    RectangleRenderer(sf::Vector2f size);
    RectangleRenderer(const RectangleRenderer& other);

    void setColor(sf::Color color);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::RectangleShape rect;
};

// TODO(#9): SpriteRenderer
class SpriteRenderer: public Renderer {
public:
    DERIVE_CLONE_COMPONENT

    SpriteRenderer(std::string texturePath);
    SpriteRenderer(const SpriteRenderer& other);

    sf::Texture getTexture() const;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
    sf::Texture texture;
    sf::Sprite  sprite;
};

#endif
