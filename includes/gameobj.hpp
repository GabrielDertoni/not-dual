#ifndef _GAMEOBJ_H_
#define _GAMEOBJ_H_

#include <cstddef>
#include <stdexcept>
#include <sstream>
#include <type_traits>
#include <deque>
#include <list>
#include <memory>
#include <mutex>
#include <atomic>
#include <string>
#include <map>

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

    // This `const` is a little lie. It actually DOES mutate the class, but it
    // uses an atomic, so operations are thread safe. We really just care about
    // cost because of race conditions anyway.
    void destroy() const;
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
    const T& getConstComponent() const
        requires std::is_base_of_v<Component, T>;

    template <class T>
    T& getComponent()
        requires std::is_base_of_v<Component, T>;

    template <class T>
    bool hasComponent() const
        requires std::is_base_of_v<Component, T>;

private:
    std::string tag;
    std::atomic<bool> shouldBeDestroyed;
    std::vector<std::unique_ptr<Component>> components;
    std::string uuid;

    friend GameObject GameObjectBuilder::build();

public:
    static std::map<std::string, class GameObjectHandle>& getGameObjects();
    static void addGameObject(GameObject gameObject);
    static void destroyAllInstances();
    static void markForDestruction(std::string id);
    static void destroyAllMarked();
    static void instantiateAllMarked();

private:
    static std::map<std::string, class GameObjectHandle> instances;

    // A list of GameObject IDs to be destroyed by the end of the frame.
    static std::mutex destroyQueueLock;
    static std::deque<std::string> destroyQueue;

    // A list of GameObjects to be instanciated by the end of the frame.
    static std::mutex instantiateQueueLock;
    static std::deque<std::shared_ptr<GameObject>> instantiateQueue;
};

class Behaviour: public Component {
protected:
    virtual void initialize(GameObject& gameObject) = 0;
    virtual void update(GameObject& gameObject) = 0;

    friend void GameObject::initialize();
    friend void GameObject::update();
};

// RAII Guard that prevents multiple references to the same GameObject.
// NOTE: It is a logic error to store a reference to inside the GameObject
// without having the guard. For instance:
//
// ```c++
// Renderer& undefinedBehaviour(GameObjectHandle& handle) {
//      GameObjectGuard guard = handle.acquire();
//      return guard->getComponent<Renderer>();
// }
// ```
//
// is undefined behavior because the reference `Renderer&` which points to
// inside the game object is still alive when the guard is destroyed and thus
// one could have two references to inside the same GameObject, which is
// undefined.
//
class GameObjectWriteGuard {
public:
    GameObject* operator->();

    ~GameObjectWriteGuard();

private:
    friend class GameObjectHandle;

    GameObjectWriteGuard(std::unique_lock<std::mutex>&& guard, std::shared_ptr<GameObject>&& gameObject);

    std::unique_lock<std::mutex> guard;
    std::shared_ptr<GameObject> gameObject;
};

class GameObjectGuard {
public:
    const GameObject* operator->();

private:
    friend class GameObjectHandle;

    GameObjectGuard(std::shared_ptr<GameObject>&& gameObject);

    std::shared_ptr<GameObject> gameObject;
};

class GameObjectHandle {
public:
    GameObjectHandle(GameObject&& gameObject);
    GameObjectHandle(std::shared_ptr<GameObject> gameObject);

    GameObjectWriteGuard acquire();
    GameObjectGuard operator->();

private:
    std::mutex writeLock;
    std::mutex sharedPtrLock;
    std::shared_ptr<GameObject> gameObject;
};

/* Implementation of generic functions */

template <class T>
const T& GameObject::getConstComponent() const
    requires std::is_base_of_v<Component, T>
{
    return ((GameObject*)this)->getComponent<T>();
}

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
bool GameObject::hasComponent() const
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
