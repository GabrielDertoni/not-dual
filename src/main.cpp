#include <iostream>
#include <semaphore>
#include <thread>
#include <chrono>
#include <deque>

#include <SFML/Graphics.hpp>

#include "includes/player.h"
#include "includes/gameobj.h"
#include "includes/globals.h"

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

Player p1(sf::Vector2f(100, 200), sf::Color::Green);
Player p2(sf::Vector2f(500, 200), sf::Color::Red);

std::deque<sf::Drawable*> drawQueue;
std::deque<sf::Event> eventQueue;

std::vector<GameObject*> gameObjects;
std::array<bool, 26> keysPressed;

bool done;

void gameLoop() {
    while (!done) {
        gameLoopStart.acquire();
        for (auto& obj : gameObjects) {
            obj->update();
        }
        gameLoopDone.release();
    }
}

int main() {
    int width = 600;
    int height = 400;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, height));
    divisionLine.setPosition(width / 2, 0);

    p1.setRotation(90);
    p2.setRotation(270);

    gameObjects.push_back(&p1);
    gameObjects.push_back(&p2);

    /*
    auto pair = spmc::channel();
    Sender<sf::Event> inputSender = pair.first;
    Receiver<sf::Event> rx1 = pair.second;
    Receiver<sf::Event> rx2 = pair.second;
    */

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

        // Push stuff to the draw queue
        drawQueue.push_back(&p1.mesh);
        drawQueue.push_back(&p2.mesh);
        drawQueue.push_back(&divisionLine);

        eventQueue.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                done = true;
            } else if (event.type == sf::Event::KeyPressed && event.key.code < 26) {
                keysPressed[event.key.code] = true;
            } else if (event.type == sf::Event::KeyReleased && event.key.code < 26) {
                keysPressed[event.key.code] = false;
            }

            eventQueue.push_back(event);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }

    gameLoopStart.release();
    gameThread.join();

    return 0;
}
