#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <list>
#include <memory>

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"

#define WIDTH  600
#define HEIGHT 400

static const BoxCollider worldCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, HEIGHT), true);

extern std::list<std::unique_ptr<GameObject>> gameObjects;

void addGameObject(std::unique_ptr<GameObject> gameObject);

void markForDestruction(std::list<std::unique_ptr<GameObject>>::iterator el);

void destroyAllMarked();

#endif
