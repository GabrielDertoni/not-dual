#ifndef _BULLET_H_
#define _BULLET_H_

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"
#include "includes/utils.hpp"

#define BULLET_SIZE       5
#define SUPER_BULLET_SIZE 10

class Bullet: public Behaviour {
public:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);

    DERIVE_CLONE_COMPONENT

    Bullet(bool isSuper);
    Bullet(const Bullet& other);

    bool isSuper;
};

#endif
