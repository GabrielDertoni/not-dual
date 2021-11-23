#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/gameobj.hpp"

class Spaceship: public Behaviour, public sf::Drawable {
public:
    Spaceship(sf::Color color, float size);
    Spaceship(const Spaceship& other);

    virtual std::unique_ptr<Component> clone();
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);

private:
    float size;
    sf::RectangleShape shape;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
