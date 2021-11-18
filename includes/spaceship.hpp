#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/gameobj.hpp"

class Spaceship: public sf::Drawable {
public:
    Spaceship(GameObject* gameObject, sf::Color color, float size);

    void update();

private:
    GameObject* gameObject;

    float size;
    sf::RectangleShape shape;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
