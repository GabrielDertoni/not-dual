#include "includes/rendering.hpp"

std::deque<const sf::Drawable*> drawQueue;
std::deque<const sf::Drawable*> nextDrawQueue;

void setupDrawQueue() {
    std::swap(drawQueue, nextDrawQueue);
    nextDrawQueue.clear();
}

void pushDrawable(const sf::Drawable* drawable) {
    nextDrawQueue.push_back(drawable);
}
