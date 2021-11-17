#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>

class Spaceship: public sf::Drawable {
public:
    sf::Vector2<float> pos;
    float ang;
    sf::Color color;

    Spaceship(sf::Vector2<float> pos, float ang, sf::Color color) :
        pos(pos),
        ang(ang),
        color(color)
    {}
    
    Spaceship(const Spaceship& other);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
