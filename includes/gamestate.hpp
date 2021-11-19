#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <cstddef>
#include <vector>
#include <iterator>
#include <memory>
#include <algorithm>
#include <type_traits>

#include <SFML/System/Vector2.hpp>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"

#define WIDTH  600
#define HEIGHT 400

static const BoxCollider worldCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, HEIGHT), true);

extern std::vector<std::unique_ptr<GameObject>> gameObjects;

void addGameObjectUnique(std::unique_ptr<GameObject> gameObject);

/*
template <class T>
void addGameObject(T gameObject)
    requires std::is_base_of<GameObject, T>::value &&
             std::is_copy_constructible<T>::value;
*/
template <class T>
void addGameObject(T gameObject);

void markForDestruction(size_t idx);

void destroyAllMarked();

std::vector<std::unique_ptr<GameObject>>& getGameObjects();

/* Implementation of generic functions */

template <class T>
void addGameObject(T gameObject) {
    addGameObjectUnique(std::make_unique<T>(gameObject));
}

#endif
