#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <list>

#include <SFML/Graphics/Drawable.hpp>
#include <memory>

class GameObject {
public:
    void destroy(std::list<std::unique_ptr<GameObject>>::iterator self);

    virtual void update() = 0;
    virtual sf::Drawable* getMesh() = 0;
};

#endif
