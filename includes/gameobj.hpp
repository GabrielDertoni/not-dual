#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <deque>
#include <list>
#include <memory>
#include <string>

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class Component {
public:
    Component();

    // Clone the component. Most implementation should just call the copy
    // constructor of the concrete class.
    virtual std::unique_ptr<Component> clone() = 0;
};

class Transform {
public:
    sf::Vector2f position;
    float angle;

    Transform(sf::Vector2f position, float angle);
    Transform(const Transform& other);

    sf::Transform getTranformMatrix();
};

class GameObject;

class GameObjectBuilder {
public:
    GameObjectBuilder(Transform transform);

    GameObjectBuilder& withTag(std::string&& tag);

    template <typename T, class... Args> 
    GameObjectBuilder& addComponent(Args&&... args);

    template <typename F> 
    GameObjectBuilder& addComponentFrom(F getComponent)
        requires std::is_invocable<F>::value
              && std::is_copy_constructible<decltype(getComponent())>::value;

    GameObject build();
    size_t registerGameObject();

private:
    Transform transform;
    std::string tag;
    std::unordered_map<size_t, std::unique_ptr<Component>> components;
};

class GameObject {
public:
    Transform transform;

public:
    GameObject(Transform transform);
    GameObject(const GameObject& other);
    GameObject(GameObject&& other);

    GameObject& operator=(GameObject &&other);

    void destroy();
    void initialize(size_t self);
    void update(size_t self);

    void setTag(const std::string& tag);
    void setPosition(sf::Vector2f position);
    void setRotation(float angle);

    const std::string& getTag() const;
    sf::Vector2f getPosition() const;
    float getRotation() const;
    float getRotationRad() const;
    sf::Vector2f getDir() const;

    template <class T, class... Args>
    T& addComponent(Args&&... args)
        requires std::is_base_of<Component, T>::value;

    template <class T>
    T& getComponent()
        requires std::is_base_of<Component, T>::value;

    template <class T>
    bool hasComponent()
        requires std::is_base_of<Component, T>::value;

private:
    std::string tag;
    bool shouldBeDestroyed;
    std::unordered_map<size_t, std::unique_ptr<Component>> components;

    void addComponentUnique(size_t id, std::unique_ptr<Component> component);

    friend GameObject GameObjectBuilder::build();

public:
    static std::vector<GameObject>& getGameObjects();
    static size_t addGameObject(GameObject gameObject);
    static void markForDestruction(size_t idx);
    static void destroyAllMarked();

private:
    static std::vector<GameObject> instances;
    static std::deque<size_t> destroyQueue;
};

class Behaviour: public Component {
protected:
    virtual void initialize(GameObject& gameObject) = 0;
    virtual void update(GameObject& gameObject) = 0;

    friend void GameObject::initialize(size_t self);
    friend void GameObject::update(size_t self);
};

/* Implementation of generic functions */

template <class T>
T& GameObject::getComponent()
    requires std::is_base_of<Component, T>::value
{
    const std::type_info& id = typeid(T);
    auto it = components.find(id.hash_code());
    if (it == components.end()) {
        std::stringstream s;
        s << "Component " << id.name() << " not found in GameObject";
        throw std::runtime_error(std::move(s.str()));
    }
    return *dynamic_cast<T*>(it->second.get());
}

template <class T>
bool GameObject::hasComponent()
    requires std::is_base_of<Component, T>::value
{
    return components.find(typeid(T).hash_code()) != components.end();
}

template <class T, class... Args>
T& GameObject::addComponent(Args&&... args) 
    requires std::is_base_of<Component, T>::value
{
    const std::type_info& info = typeid(T);
    auto [it, ok] = components.insert(std::make_pair(info.hash_code(), std::make_unique<T>(args...)));

    if (!ok) {
        std::stringstream s;
        s << "Component " << info.name() << " was already present in GameObject";
        throw std::runtime_error(std::move(s.str()));
    }

    return *dynamic_cast<T*>(it->second.get());
}

/* GameObjectBuilder */


template <typename T, class... Args> 
GameObjectBuilder& GameObjectBuilder::addComponent(Args&&... args) {
    const std::type_info& info = typeid(T);
    auto [it, ok] = components.insert(std::make_pair(info.hash_code(), std::make_unique<T>(args...)));

    if (!ok) {
        std::stringstream s;
        s << "Component " << info.name() << " was already present in GameObject";
        throw std::runtime_error(std::move(s.str()));
    }
    return *this;
}

template <typename F>
GameObjectBuilder& GameObjectBuilder::addComponentFrom(F getComponent)
    requires std::is_invocable<F>::value
          && std::is_copy_constructible<decltype(getComponent())>::value
{
    const std::type_info& info = typeid(decltype(getComponent()));
    auto [it, ok] = components.insert(
            std::make_pair(info.hash_code(),
                           std::make_unique<decltype(getComponent())>(getComponent())));

    if (!ok) {
        std::stringstream s;
        s << "Component " << info.name() << " was already present in GameObject";
        throw std::runtime_error(std::move(s.str()));
    }
    return *this;
}


#endif
