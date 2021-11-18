#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <list>
#include <memory>

#include "includes/gameobj.hpp"

extern std::list<std::unique_ptr<GameObject>> gameObjects;

void addGameObject(std::unique_ptr<GameObject> gameObject);

void markForDestruction(std::list<std::unique_ptr<GameObject>>::iterator el);

void destroyAllMarked();

#endif
