#include <iostream>
#include <deque>
#include <vector>
#include <memory>

#include "includes/gamestate.hpp"
#include "includes/gameobj.hpp"

std::vector<std::unique_ptr<GameObject>> gameObjects;

static std::deque<size_t> destroyQueue;

void markForDestruction(size_t idx) {
    destroyQueue.push_back(idx);
}

void destroyAllMarked() {
    while (!destroyQueue.empty()) {
        // Swap with last element end remove. O(1)
        std::swap(gameObjects[destroyQueue.front()], *--gameObjects.end());
        gameObjects.pop_back();
        destroyQueue.pop_front();
    }
}

void addGameObjectUnique(std::unique_ptr<GameObject> gameObject) {
    gameObjects.push_back(std::move(gameObject));
}


std::vector<std::unique_ptr<GameObject>>& getGameObjects() {
    return gameObjects;
}
