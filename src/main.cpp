#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>
#include <list>

#include <SFML/Graphics.hpp>

#include "includes/player.hpp"
#include "includes/gameobj.hpp"
#include "includes/gamestate.hpp"
#include "includes/input.hpp"

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

std::deque<const sf::Drawable*> drawQueue;

bool done;

bool gameIsOver = false;

void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done) break;

        auto end = gameObjects.end();
        for (auto it = gameObjects.begin(); it != end; it++) {
            (*it)->callUpdate(it);
        }

        destroyAllMarked();

        if (gameIsOver) {
            
        }
        gameLoopDone.release();
    }
}

void populateEventQueue(sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            done = true;
        } else if (event.type == sf::Event::KeyPressed) {
            input::registerKeyPress(event.key);
        } else if (event.type == sf::Event::KeyReleased) {
            input::registerKeyRelease(event.key);
        }
    }
}

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, HEIGHT));
    divisionLine.setPosition(WIDTH / 2, 0);

    auto p1 = std::make_unique<Player>(
        Transform(sf::Vector2f(100, 200), 0),
        sf::Color::Green,
        &wasdController,
        BoxCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH / 2, HEIGHT), true)
    );
    p1->setTag("Player1");
    addGameObject(std::move(p1));

    auto p2 = std::make_unique<Player>(
        Transform(sf::Vector2f(500, 200), 180),
        sf::Color::Red,
        &arrowsController,
        BoxCollider(sf::Vector2f(WIDTH / 2, 0), sf::Vector2f(WIDTH, HEIGHT), true)
    );
    p2->setTag("Player2");
    addGameObject(std::move(p2));

    std::thread gameThread(gameLoop);

    done = false;

    while (window.isOpen()) {
        // Game loop starts
        gameLoopStart.release();

        window.clear();
        while (!drawQueue.empty()) {
            window.draw(*drawQueue.front());
            drawQueue.pop_front();
        }
        window.display();

        // Game loop ends
        gameLoopDone.acquire();

        if (done) {
            window.close();
            break;
        }

        // Push stuff to the draw queue
        for (auto& obj : gameObjects) {
            drawQueue.push_back(obj->getMesh());
        }
        drawQueue.push_back(&divisionLine);

        populateEventQueue(window);

        std::this_thread::sleep_for(frameTimeBudget);
    }

    gameLoopStart.release();
    gameThread.join();

    return 0;
}
