#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <cstddef>
#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/Drawable.hpp>

class Component {};

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
    void callUpdate(size_t self);
    void setTag(const std::string& tag);

    void setPosition(sf::Vector2f position);
    void setRotation(float angle);

    const std::string& getTag() const;
    sf::Vector2f getPosition() const;
    float getRotation() const;
    float getRotationRad() const;
    sf::Vector2f getDir() const;

    template <class T>
    T* tryCast();

    template <class T>
    void addComponent(T component);

    template <class T>
    T* getComponent();

public:
    virtual void update() = 0;
    virtual const sf::Drawable* getMesh() const = 0;

protected:
    std::string tag;

private:
    bool shouldBeDestroyed;
    std::unordered_map<std::size_t, std::unique_ptr<Component>> components;
};

/* Implementation of generic functions */

template <class T>
T* GameObject::tryCast() {
    return dynamic_cast<T*>(this);
}

template <class T>
T* GameObject::getComponent() {
    const std::type_info& id = typeid(T);
    auto it = components.find(id.hash_code());
    if (it == components.end()) {
        return nullptr;
    }
    return dynamic_cast<T>(it->second.get());
}

template <class T>
void GameObject::addComponent(T component) {
    components.insert(typeid(T).hash_code(), std::make_unique<T>(component));
}

#endif
