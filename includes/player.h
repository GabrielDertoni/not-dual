#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "spaceship.h"

class Player: public sf::Drawable {
public:
    sf::Vector2<float> pos;
    float ang;
    sf::Vector2<float> vel;
    sf::Vector2<float> acc;

    Spaceship mesh;

    Player(sf::Vector2<float> pos, sf::Color color) :
        pos(pos),
        ang(0),
        vel(0, 0),
        acc(0, 0),
        color(color) {}

    void update();
    void setRotation(float ang);

private:
    sf::Color color;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif
