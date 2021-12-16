#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/gameobj.hpp"
#include "includes/renderer.hpp"

class SpaceshipRenderer: public Renderer {
public:
    DERIVE_CLONE_COMPONENT

    SpaceshipRenderer(std::string texturePath);
    SpaceshipRenderer(const SpaceshipRenderer& other);

    sf::Color getColor() const;

private:
    sf::Texture texture;
    sf::Sprite  shape;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
