#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/Drawable.hpp>

class Transform {
public:
    sf::Vector2f position;
    float angle;

    Transform(sf::Vector2f position, float angle);
    Transform(const Transform& other);
};

class GameObject {
public:
    Transform transform;

public:
    GameObject(Transform transform);

    void destroy();
    void callUpdate(std::list<std::unique_ptr<GameObject>>::iterator self);
    void setTag(std::string tag);

    void setPosition(sf::Vector2f position);
    void setRotation(float angle);

    const std::string& getTag() const;
    sf::Vector2f getPosition() const;
    float getRotation() const;
    float getRotationRad() const;
    sf::Vector2f getDir() const;

    // template <class T>
    // T* tryCast();

public:
    virtual void update() = 0;
    virtual const sf::Drawable* getMesh() const = 0;

protected:
    std::string tag;

private:
    bool shouldBeDestroyed;
};

/* Implementation of generic functions */

/*
template <class T>
T* tryCast() {
    return dynamic_cast<T*>(this);
}
*/

#endif
