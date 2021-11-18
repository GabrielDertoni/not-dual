#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/Drawable.hpp>

class GameObject {
public:
    GameObject() :
        shouldBeDestroyed(false) {}

    void destroy();
    void callUpdate(std::list<std::unique_ptr<GameObject>>::iterator self);
    void setTag(std::string tag);
    std::string& getTag();

    virtual void update() = 0;
    virtual sf::Drawable* getMesh() = 0;

protected:
    std::string tag;

private:
    bool shouldBeDestroyed;
};

#endif
