#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>
#include <list>

#include <SFML/Graphics.hpp>

#include "includes/gameobj.hpp"
#include "includes/player.hpp"
#include "includes/renderer.hpp"
#include "includes/rigidbody.hpp"
#include "includes/input.hpp"
#include "includes/settings.hpp"
#include "includes/rendering.hpp"
#include "includes/superpower.hpp"
#include "includes/time.hpp"

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(1);

bool done;

bool gameIsOver = false;

static const sf::Vector2f leftPlayerStartPos = sf::Vector2f(100, HEIGHT / 2);
static const sf::Vector2f rightPlayerStartPos = sf::Vector2f(WIDTH - 100, HEIGHT / 2);

void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done) break;

        std::vector<GameObject>& objs = GameObject::getGameObjects();
        for (size_t i = 0; i < objs.size(); i++) {
            objs[i].update(i);
        }

        // Destruction MUST HAPPEN BEFORE instantiation.
        GameObject::destroyAllMarked();
        GameObject::instantiateAllMarked();

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

std::deque<std::pair<sf::Transform, std::unique_ptr<Renderer>>> dq;

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, HEIGHT));
    divisionLine.setPosition(WIDTH / 2, 0);

    GameObjectBuilder(Transform(leftPlayerStartPos, 0))
        .withTag("Player1")
        .addComponent<Player>(&wasdController, Player::LEFT)
        .addComponent<Renderer>(Spaceship(sf::Color::Green, PLAYER_SIZE))
        .addComponent<BoxCollider>(BoxCollider(-playerSize, playerSize))
        .addComponent<RigidBody>(1.0f)
        .registerGameObject();

    GameObjectBuilder(Transform(rightPlayerStartPos, 0))
        .withTag("Player2")
        .addComponent<Player>(&arrowsController, Player::RIGHT)
        .addComponent<Renderer>(Spaceship(sf::Color::Blue, PLAYER_SIZE))
        .addComponent<BoxCollider>(BoxCollider(-playerSize, playerSize))
        .addComponent<RigidBody>(1.0f)
        .registerGameObject();

    GameObjectBuilder(Transform(sf::Vector2f(WIDTH/2 - 1, 0), 0))
        .addComponent<Renderer>(RectangleShape(sf::Vector2f(2, HEIGHT)))
        .registerGameObject();

    auto superPowerPos1 = sf::Vector2f(250, HEIGHT / 2);
    auto superPowerPos2 = sf::Vector2f(450, HEIGHT / 2);
    GameObjectBuilder(Transform(superPowerPos1, 0))
        .addComponent<Spawner>(getNow())
        .registerGameObject();
    GameObjectBuilder(Transform(superPowerPos2, 0))
        .addComponent<Spawner>(getNow())
        .registerGameObject();

    /*
    std::vector<GameObject>& objs = GameObject::getGameObjects();
    for (size_t i = 0; i < objs.size(); i++) {
        objs[i].initialize(i);
    }
    */

    std::thread gameThread(gameLoop);

    done = false;

    while (window.isOpen()) {
        // Game loop starts
        gameLoopStart.release();

        window.clear();
        while (!dq.empty()) {
            auto& [transform, drawable] = dq.front();
            sf::RenderStates states = sf::RenderStates::Default;
            states.transform *= transform;
            window.draw(*drawable, states);
            dq.pop_front();
        }
        window.display();

        // Game loop ends
        gameLoopDone.acquire();

        if (done) {
            window.close();
            break;
        }

        // Push stuff to the draw queue
        for (auto& obj : GameObject::getGameObjects()) {
            if (obj.hasComponent<Renderer>()) {
                std::unique_ptr<Component> comp = obj.getComponent<Renderer>().clone();
                std::unique_ptr<Renderer> renderer(dynamic_cast<Renderer*>(comp.release()));
                dq.push_back(std::make_pair(obj.transform.getTranformMatrix(), std::move(renderer)));
            }
        }

        // setupDrawQueue();
        // dq.push_back(std::make_pair(sf::Transform(), &divisionLine));

        populateEventQueue(window);

        std::this_thread::sleep_for(frameTimeBudget);
    }

    gameLoopStart.release();
    gameThread.join();

    return 0;
}
