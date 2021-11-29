#ifndef _UTILS_H_
#define _UTILS_H_

#include <type_traits>

#include <SFML/Graphics/Color.hpp>

sf::Color colorLerp(sf::Color start, sf::Color end, float lerp);

// Helper macros for defining, implementing or both the `clone` function. This
// can safe some unecessary copy paste.

#define DECL_CLONE_COMPONENT() \
    virtual std::unique_ptr<Component> className::clone();

#define IMPL_CLONE_COMPONENT(className)                                        \
    std::unique_ptr<Component> className::clone() {                            \
        return std::make_unique<className>(*this);                             \
    }

#define DERIVE_CLONE_COMPONENT                                                     \
    virtual std::unique_ptr<Component> clone() {                                   \
        return std::make_unique<std::remove_pointer<decltype(this)>::type>(*this); \
    }

#endif
