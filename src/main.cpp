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
#include "includes/mainmenu.hpp"

const std::chrono::duration frameTimeBudget = std::chrono::milliseconds(17);
static std::chrono::duration deltaTime = std::chrono::milliseconds(17);

std::binary_semaphore gameLoopStart(0);
std::binary_semaphore gameLoopDone(0);

static bool done;
static bool gameIsOver = false;
static std::deque<std::pair<sf::Transform, std::unique_ptr<Renderer>>> drawQueue;

static const sf::Vector2f leftPlayerStartPos = sf::Vector2f(100, HEIGHT / 2);
static const sf::Vector2f rightPlayerStartPos = sf::Vector2f(WIDTH - 100, HEIGHT / 2);

void mainMenu(bool showGameOverText);

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

        bool player1Alive = false;
        bool player2Alive = false;

        for (auto [id, obj] : GameObject::getGameObjects()) {
            if(obj->getTag() == "Player1"){
                player1Alive = true;
            }

            if(obj->getTag() == "Player2"){
                player2Alive = true;
            }
        }

        if (!player1Alive || !player2Alive) {
            gameLoopDone.release();
            gameIsOver = true;
            return;
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

void playGame(){
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Not-Dual", sf::Style::Default, settings);

    window.setKeyRepeatEnabled(false);

    sf::RectangleShape divisionLine(sf::Vector2f(1, HEIGHT));
    divisionLine.setPosition(WIDTH / 2, 0);

    sf::Transform p1SpriteOffset;
    p1SpriteOffset.translate(-15, -20);

    sf::Transform p2SpriteOffset;
    p2SpriteOffset.translate(-20, -20);

    GameObjectBuilder(Transform(leftPlayerStartPos, 0))
        .withTag("Player1")
        .addComponent<Player>(&wasdController, Player::LEFT)
        .addComponent<SpriteRenderer>(LS_PATH, p1SpriteOffset)
        .addComponent<BoxCollider>(-playerSize, playerSize)
        .addComponent<RigidBody>(1.0f, 0.98)
        .registerGameObject();

    GameObjectBuilder(Transform(rightPlayerStartPos, 0))
        .withTag("Player2")
        .addComponent<Player>(&arrowsController, Player::RIGHT)
        .addComponent<SpriteRenderer>(RS_PATH, p2SpriteOffset)
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

        // Push stuff to the draw queue
        for (auto [id, obj] : GameObject::getGameObjects()) {
            if (obj->hasComponent<Renderer>()) {
                std::unique_ptr<Component> comp = obj->getComponent<Renderer>().clone();
                std::unique_ptr<Renderer> renderer(dynamic_cast<Renderer*>(comp.release()));
                drawQueue.push_back(std::make_pair(obj->transform.getTransformMatrix(), std::move(renderer)));
            }
        }

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

        if (done || gameIsOver) {
            window.close();
            break;
        }

        populateEventQueue(window);

        Timestamp frameEnd = getNow();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);
        if (deltaTime < frameTimeBudget) {
            std::this_thread::sleep_for(frameTimeBudget - deltaTime);
        }
    }
    done = true;
    gameLoopStart.release();
    gameThread.join();

    if(gameIsOver){
        mainMenu(true);
    }
}

void howToPlay() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Game window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "How to play", sf::Style::Default, settings);

    // How to play UI objects
    sf::Texture howToPlayTexture;
    howToPlayTexture.loadFromFile("resources/howtoplay.png");
    howToPlayTexture.setRepeated(true);
    sf::Sprite howToPlaySprite(howToPlayTexture);

    while (window.isOpen())
    {
        sf::Event aEvent;
        while (window.pollEvent(aEvent)){
            if (aEvent.type == sf::Event::Closed){
                window.close();
            }
            if (aEvent.type == sf::Event::KeyPressed && aEvent.key.code == sf::Keyboard::Escape){
                // FIXME: This should work!
                // window.close();
            }
        }

        window.clear(sf::Color(49,21,58));
        window.draw(howToPlaySprite);
        window.display();
    }
}

void mainMenu(bool showGameOverText) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Building the Main Menu window
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Not-Dual", sf::Style::Default, settings);
    MainMenu mainMenu(window.getSize().x, window.getSize().y, showGameOverText + 1);

    // Setting the window icon
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close the window
            if (event.type == sf::Event::Closed){
                window.close();
            }

            // Check for key events
            if (event.type == sf::Event::KeyReleased){
                // Treat all valid key events for GUI interactions
                switch (event.key.code){
                // Move Up
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    mainMenu.MoveUp(3);
                    break;

                // Move Down
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    mainMenu.MoveDown(3);
                    break;

                // Select option
                case sf::Keyboard::Return:
                case sf::Keyboard::Space:
                {
                    int opt = mainMenu.InterfacePressed();
                    if (opt == 0) {
                        window.close();
                        playGame();
                    } else if (opt == 1) {
                        howToPlay();
                    } else {
                        window.close();
                    }
                    break;
                }

                default:
                    break;
                }
            }
        }

        window.clear(sf::Color(49,21,58));
        mainMenu.draw(window);
        window.display();
    }
}



int main() {
    mainMenu(0);
    return 0;
}
