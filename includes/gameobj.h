#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <SFML/Graphics/Drawable.hpp>

class GameObject {
public:
    virtual void update() = 0;
    virtual sf::Drawable* getMesh() = 0;
};

#endif
