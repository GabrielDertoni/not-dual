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
#include "includes/superpower.hpp"
#include "includes/time.hpp"

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);
static std::chrono::duration deltaTime = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(0);

static bool done;
static bool gameIsOver = false;
static std::deque<std::pair<sf::Transform, std::unique_ptr<Renderer>>> drawQueue;

static const sf::Vector2f leftPlayerStartPos = sf::Vector2f(100, HEIGHT / 2);
static const sf::Vector2f rightPlayerStartPos = sf::Vector2f(WIDTH - 100, HEIGHT / 2);

void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done) break;

        for (auto& [id, obj] : GameObject::getGameObjects()) {
            obj->update();
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

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Not-Dual", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, HEIGHT));
    divisionLine.setPosition(WIDTH / 2, 0);

    GameObjectBuilder(Transform(leftPlayerStartPos, 0))
        .withTag("Player1")
        .addComponent<Player>(&wasdController, Player::LEFT)
        .addComponent<SpaceshipRenderer>(LS_PATH)
        .addComponent<BoxCollider>(-playerSize, playerSize)
        .addComponent<RigidBody>(1.0f, 0.98)
        .registerGameObject();

    GameObjectBuilder(Transform(rightPlayerStartPos, 0))
        .withTag("Player2")
        .addComponent<Player>(&arrowsController, Player::RIGHT)
        .addComponent<SpaceshipRenderer>(RS_PATH)
        .addComponent<BoxCollider>(-playerSize, playerSize)
        .addComponent<RigidBody>(1.0f, 0.98)
        .registerGameObject();

    GameObjectBuilder(Transform(sf::Vector2f(WIDTH/2 - 1, 0), 0))
        .addComponent<RectangleRenderer>(sf::Vector2f(2, HEIGHT))
        .registerGameObject();

    GameObjectBuilder(Transform(sf::Vector2f(250, HEIGHT / 2), 0))
        .addComponent<PowerSpawner>()
        .registerGameObject();
    GameObjectBuilder(Transform(sf::Vector2f(450, HEIGHT / 2), 0))
        .addComponent<PowerSpawner>()
        .registerGameObject();

    sf::Texture bg;
    bg.loadFromFile(BG_PATH);
    sf::Sprite bgSprite(bg);


    std::thread gameThread(gameLoop);

    done = false;

    while (window.isOpen()) {
        Timestamp frameStart = getNow();
        // Game loop starts
        gameLoopStart.release();

        window.clear();

        window.draw(bgSprite);
        while (!drawQueue.empty()) {
            auto& [transform, drawable] = drawQueue.front();
            sf::RenderStates states = sf::RenderStates::Default;
            states.transform *= transform;
            window.draw(*drawable, states);
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
        for (auto [id, obj] : GameObject::getGameObjects()) {
            if (obj->hasComponent<Renderer>()) {
                std::unique_ptr<Component> comp = obj->getComponent<Renderer>().clone();
                std::unique_ptr<Renderer> renderer(dynamic_cast<Renderer*>(comp.release()));
                drawQueue.push_back(std::make_pair(obj->transform.getTransformMatrix(), std::move(renderer)));
            }
        }

        populateEventQueue(window);

        Timestamp frameEnd = getNow();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        if (deltaTime < frameTimeBudget) {
            std::this_thread::sleep_for(frameTimeBudget - deltaTime);
        }
    }

    gameLoopStart.release();
    gameThread.join();

    return 0;
}
