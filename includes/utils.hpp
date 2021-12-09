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

// class_t is the type of the class itself. So, if `this` has type `const *BoxCollider`,
// for example, then `class_t` will be just `BoxCollider`. We first remove the pointer
// and then the `const` of the type of `this`.
#define DERIVE_CLONE_COMPONENT                                                      \
    virtual std::unique_ptr<Component> clone() const {                              \
        typedef std::remove_const_t<std::remove_pointer_t<decltype(this)>> class_t; \
        return std::make_unique<class_t>(*this);                                    \
    }

#endif
