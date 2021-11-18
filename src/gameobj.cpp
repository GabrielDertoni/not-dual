
#include "includes/gameobj.hpp"
#include "includes/gamestate.hpp"


void GameObject::destroy(std::list<std::unique_ptr<GameObject>>::iterator self) {
    markForDestruction(self);
}
