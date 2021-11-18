#include <iostream>
#include "includes/gameobj.hpp"
#include "includes/gamestate.hpp"


void GameObject::destroy() {
    shouldBeDestroyed = true;
}

void GameObject::callUpdate(std::list<std::unique_ptr<GameObject>>::iterator self) {
    update();

    if (shouldBeDestroyed) {
        markForDestruction(self);
    }
}

void GameObject::setTag(std::string tag) {
    this->tag = tag;
}

std::string& GameObject::getTag() {
    return tag;
}
