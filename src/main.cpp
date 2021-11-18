#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>

#include <SFML/Graphics.hpp>

#include "includes/player.hpp"
#include "includes/gameobj.hpp"
#include "includes/gamestate.hpp"
#include "includes/input.hpp"

#define WIDTH  600
#define HEIGHT 400

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

std::deque<sf::Drawable*> drawQueue;

bool done;

bool gameIsOver = false;

void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done) break;

        for (auto& obj : gameObjects) {
            obj->update();
        }

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

    addGameObject(Player::create_unique<WASDController>(
        sf::Vector2f(100, 200),
        sf::Color::Green,
        BoxCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH / 2, HEIGHT), true)
    ));

    addGameObject(Player::create_unique<ArrowsController>(
        sf::Vector2f(500, 200),
        sf::Color::Red,
        BoxCollider(sf::Vector2f(WIDTH / 2, 0), sf::Vector2f(WIDTH, HEIGHT), true)
    ));

    std::thread gameThread(gameLoop);

    done = false;

    while (window.isOpen()) {
        // Game loop starts
        gameLoopStart.release();

        window.clear();
        while (!drawQueue.empty()) {
            sf::Drawable* shape = drawQueue.front();
            window.draw(*shape);
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
