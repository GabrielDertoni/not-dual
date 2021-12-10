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
#include <unordered_map>

#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


class Component {
public:
    Component();

    // Clone the component. Most implementation should just call the copy
    // constructor of the concrete class.
    virtual std::unique_ptr<Component> clone() const = 0;
};

class Transform {
public:
    sf::Vector2f position;
    float angle;

    Transform(sf::Vector2f position, float angle);
    Transform(const Transform& other);

    sf::Transform getTransformMatrix() const;
};

Transform operator* (const Transform& lhs, const Transform& rhs);

class GameObject;

class GameObjectBuilder {
public:
    GameObjectBuilder(Transform transform);
    GameObjectBuilder(const GameObjectBuilder& other);

    GameObjectBuilder& withTag(std::string&& tag);

    template <typename T, class... Args>
    GameObjectBuilder& addComponent(Args&&... args);

    template <typename T>
    GameObjectBuilder& addComponentUnique(std::unique_ptr<T> component);

    template <typename F>
    GameObjectBuilder& addComponentFrom(F getComponent)
        requires std::is_invocable_v<F>
              && std::is_copy_constructible_v<decltype(getComponent())>
              && std::is_base_of_v<Component, decltype(getComponent())>;

    template <typename F>
    GameObjectBuilder& mapTransform(F fn)
        requires std::is_invocable_v<F, Transform>
              && std::is_same_v<Transform, std::invoke_result_t<F, Transform>>;

    GameObjectBuilder copy();
    GameObject build();
    void registerGameObject();

private:
    Transform transform;
    std::string tag;
    std::vector<std::unique_ptr<Component>> components;
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
    void initialize();
    void update();

    void setTag(const std::string& tag);
    void setPosition(sf::Vector2f position);
    void setRotation(float angle);

    const std::string& getTag() const;
    sf::Vector2f getPosition() const;
    float getRotation() const;
    float getRotationRad() const;
    sf::Vector2f getDir() const;
    const std::string& getUUID() const;

    template <class T, class... Args>
    T& addComponent(Args&&... args)
        requires std::is_base_of_v<Component, T>;

    void addComponentUnique(std::unique_ptr<Component> component);

    template <class T>
    T& getComponent()
        requires std::is_base_of_v<Component, T>;

    template <class T>
    bool hasComponent()
        requires std::is_base_of_v<Component, T>;

private:
    std::string tag;
    bool shouldBeDestroyed;
    std::vector<std::unique_ptr<Component>> components;
    std::string uuid;

    friend GameObject GameObjectBuilder::build();

public:
    static std::unordered_map<std::string, std::shared_ptr<GameObject>>& getGameObjects();
    static void addGameObject(GameObject gameObject);
    static void markForDestruction(std::string id);
    static void destroyAllMarked();
    static void instantiateAllMarked();

private:
    // static std::vector<GameObject> instances;
    static std::unordered_map<std::string, std::shared_ptr<GameObject>> instances;

    // A list of GameObject IDs to be destroyed by the end of the frame.
    static std::deque<std::string> destroyQueue;

    // A list of GameObjects to be instanciated by the end of the frame.
    static std::deque<std::shared_ptr<GameObject>> instantiateQueue;
};

class Behaviour: public Component {
protected:
    virtual void initialize(GameObject& gameObject) = 0;
    virtual void update(GameObject& gameObject) = 0;

    friend void GameObject::initialize();
    friend void GameObject::update();
};

/* Implementation of generic functions */

template <class T>
T& GameObject::getComponent()
    requires std::is_base_of_v<Component, T>
{
    const std::type_info& id = typeid(T);
    for (auto& component : components) {
        T* ptr = dynamic_cast<T*>(component.get());
        if (ptr != nullptr) return *ptr;
    }

    std::stringstream s;
    s << "Component " << id.name() << " not found in GameObject";
    throw std::runtime_error(std::move(s.str()));
}

template <class T>
bool GameObject::hasComponent()
    requires std::is_base_of_v<Component, T>
{
    for (auto& component : components) {
        T* ptr = dynamic_cast<T*>(component.get());
        if (ptr != nullptr) return true;
    }
    return false;
}

template <class T, class... Args>
T& GameObject::addComponent(Args&&... args)
    requires std::is_base_of_v<Component, T>
{
    std::unique_ptr<T> comp = std::make_unique<T>(args...);
    T* ptr = comp.get();
    std::unique_ptr<Component> c = comp;
    addComponentUnique(std::move(comp));
    return *ptr;
}

/* GameObjectBuilder */

template <typename T, class... Args>
GameObjectBuilder& GameObjectBuilder::addComponent(Args&&... args) {
    components.push_back(std::make_unique<T>(args...));
    return *this;
}

template <typename T>
GameObjectBuilder& GameObjectBuilder::addComponentUnique(std::unique_ptr<T> component) {
    components.push_back(std::move(component));
    return *this;
}

template <typename F>
GameObjectBuilder& GameObjectBuilder::addComponentFrom(F getComponent)
    requires std::is_invocable_v<F>
          && std::is_copy_constructible_v<decltype(getComponent())>
          && std::is_base_of_v<Component, decltype(getComponent())>
{
    components.push_back(std::make_unique<decltype(getComponent())>(getComponent()));
    return *this;
}


template <typename F>
GameObjectBuilder& GameObjectBuilder::mapTransform(F fn)
    requires std::is_invocable_v<F, Transform>
          && std::is_same_v<Transform, std::invoke_result_t<F, Transform>>
{
    this->transform = fn((Transform&)transform);
    return *this;
}


#endif
