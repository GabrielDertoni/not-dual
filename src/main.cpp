#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>

#include <SFML/Graphics.hpp>

#include "../includes/player.h"
#include "../includes/bullet.h"
#include "../includes/gameobj.h"
#include "../includes/globals.h"

#define WIDTH  600
#define HEIGHT 400

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

Player p1 = Player::create<WASDController>(
    sf::Vector2f(100, 200),
    sf::Color::Green,
    BoxCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH / 2, HEIGHT), true)
);
Player p2 = Player::create<ArrowsController>(
    sf::Vector2f(500, 200),
    sf::Color::Red,
    BoxCollider(sf::Vector2f(WIDTH / 2, 0), sf::Vector2f(WIDTH, HEIGHT), true)
);

Bullet b = Bullet::create(
    sf::Vector2f(150, 200),
    sf::Vector2f(2, 2),
    0,
    sf::Color::White,
    BoxCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH, HEIGHT), true)
);

std::deque<sf::Drawable*> drawQueue;
std::deque<sf::Event> eventQueue;

std::vector<GameObject*> gameObjects;
std::array<bool, sf::Keyboard::KeyCount> keysPressed;

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
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, HEIGHT));
    divisionLine.setPosition(WIDTH / 2, 0);

    p1.setRotation(45);
    p2.setRotation(315);

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
        drawQueue.push_back(p1.getMesh());
        drawQueue.push_back(p2.getMesh());
        drawQueue.push_back(&divisionLine);

        eventQueue.clear();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                done = true;
            } else if (event.type == sf::Event::KeyPressed) {
                keysPressed[event.key.code] = true;
            } else if (event.type == sf::Event::KeyReleased) {
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
