#include <deque>
#include <list>
#include <memory>

#include "includes/gamestate.hpp"
#include "includes/gameobj.hpp"

std::list<std::unique_ptr<GameObject>> gameObjects;

static std::deque<std::list<std::unique_ptr<GameObject>>::iterator> destroyQueue;

void markForDestruction(std::list<std::unique_ptr<GameObject>>::iterator el) {
    destroyQueue.push_back(el);
}

void destroyAllMarked() {
    while (!destroyQueue.empty()) {
        auto el = destroyQueue.front();
        destroyQueue.pop_front();
        gameObjects.erase(el);
    }
}

void addGameObject(std::unique_ptr<GameObject> gameObject) {
    gameObjects.push_back(std::move(gameObject));
}
