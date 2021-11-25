#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <iostream>
#include <memory>
#include <type_traits>

#include <SFML/Graphics/Drawable.hpp>

#include "includes/gameobj.hpp"
#include "includes/rendering.hpp"

template <typename T> requires std::is_base_of<sf::Drawable, T>::value
                            && std::is_copy_constructible<T>::value
                            && std::is_move_constructible<T>::value
class Renderer: public Behaviour {
public:
    Renderer(T drawable) :
        drawable(std::make_unique<T>(drawable))
    {}

    Renderer(const Renderer<T>& other) :
        drawable(std::make_unique<T>(*other.drawable))
    {
        printf("Copied\n");
    }

    virtual std::unique_ptr<Component> clone() {
        return std::make_unique<Renderer>(*this);
    }

protected:
    virtual void initialize(GameObject& gameObject) {
        drawable->setTransformMatrix(gameObject.transform.getTranformMatrix());
    }

    virtual void update(GameObject& gameObject) {
        // drawable->setTransformMatrix(gameObject.transform.getTranformMatrix());
        pushDrawable(drawable.get());
    }

private:
    std::unique_ptr<T> drawable;
};

#endif
