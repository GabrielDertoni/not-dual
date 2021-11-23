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
#include "includes/input.hpp"
#include "includes/settings.hpp"

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

std::deque<const sf::Drawable*> drawQueue;

bool done;

bool gameIsOver = false;

static const BoxCollider leftCollider  = BoxCollider(sf::Vector2f(0, 0), sf::Vector2f(WIDTH / 2, HEIGHT), true);
static const BoxCollider rightCollider = BoxCollider(sf::Vector2f(WIDTH / 2, 0), sf::Vector2f(WIDTH, HEIGHT), true);

static const sf::Vector2f leftPlayerStartPos = sf::Vector2f(100, HEIGHT / 2);
static const sf::Vector2f rightPlayerStartPos = sf::Vector2f(WIDTH - 100, HEIGHT / 2);

void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done) break;

        auto objs = GameObject::getGameObjects();
        for (size_t i = 0; i < objs.size(); i++) {
            objs[i].update(i);
        }

        GameObject::destroyAllMarked();

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

    GameObject p1(Transform(leftPlayerStartPos, 0));
    p1.setTag("Player1");
    p1.addComponent<Player>(&wasdController, leftCollider);
    p1.addComponent<Spaceship>(sf::Color::Green, PLAYER_SIZE);
    p1.addComponent<BoxCollider>(BoxCollider(-playerSize, playerSize));
    GameObject::addGameObject(std::move(p1));

    GameObject p2(Transform(rightPlayerStartPos, 0));
    p2.setTag("Player2");
    p2.addComponent<Player>(&arrowsController, rightCollider);
    p2.addComponent<Spaceship>(sf::Color::Blue, PLAYER_SIZE);
    p1.addComponent<BoxCollider>(rightCollider);
    GameObject::addGameObject(std::move(p2));

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
        /*
        for (auto& obj : GameObject::getGameObjects()) {
            drawQueue.push_back(obj.getMesh());
        }
        */
        drawQueue.push_back(&divisionLine);

        populateEventQueue(window);

        std::this_thread::sleep_for(frameTimeBudget);
    }

    gameLoopStart.release();
    gameThread.join();

    return 0;
}
