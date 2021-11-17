#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include <SFML/Graphics/Drawable.hpp>

class Spaceship: public sf::Drawable {
public:
    Spaceship() {}
    Spaceship(const Spaceship& other);

private:
    sf::Color color;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
