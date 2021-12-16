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


void gameLoop() {
    while (!done) {
        while (!gameLoopStart.try_acquire_for(frameTimeBudget) && !done);
        if (done)break;

        for (auto& [id, obj] : GameObject::getGameObjects()) {
            obj->update();
        }

        // Destruction MUST HAPPEN BEFORE instantiation.
        GameObject::destroyAllMarked();
        GameObject::instantiateAllMarked();

        if (gameIsOver) {
            // Game ends, return to menu (or create a 'Game Over' window)
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


void startGame(sf::RenderWindow& window, sf::RenderWindow& howToPlay, sf::RenderWindow& gameMenu){

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
        howToPlay.close();
        gameMenu.close();
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
}


void switchWindow(int x, sf::RenderWindow &gameMenu){

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    
    // Game window    
    sf::RenderWindow playGame(sf::VideoMode(WIDTH, HEIGHT), "Not-Dual", sf::Style::Default, settings);
    sf::RenderWindow howToPlay(sf::VideoMode(WIDTH, HEIGHT), "How to play", sf::Style::Default, settings);


    // How to play UI objects
    sf::Texture howToPlayTexture;
    howToPlayTexture.loadFromFile("resources/howtoplay.png");
    howToPlayTexture.setRepeated(true);
    sf::Sprite howToPlaySprite(howToPlayTexture);

    switch (x)
    {

        // Switch to playGame
        case 0:
            startGame(playGame, howToPlay, gameMenu);
            break;
        
        // Switch to howToPlay
        case 1:
            while (howToPlay.isOpen())
            {
                sf::Event aEvent;
                while (howToPlay.pollEvent(aEvent)){
                    if (aEvent.type == sf::Event::Closed){
                        howToPlay.close();
                    }
                    if (aEvent.type == sf::Event::KeyPressed){
                        if (aEvent.key.code == sf::Keyboard::Escape){
                            howToPlay.close();
                        }
                    }   

                }

                playGame.close();
                howToPlay.clear(sf::Color(49,21,58));
                howToPlay.draw(howToPlaySprite);
                howToPlay.display();
            }
            break;


        // Quit game
        case 2:
            gameMenu.close();
            break;


    default:
        break;
    }
}



int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    // Building the Main Menu window
    sf::RenderWindow gameMenu(sf::VideoMode(WIDTH, HEIGHT), "Not-Dual", sf::Style::Default, settings);
    MainMenu mainMenu(gameMenu.getSize().x, gameMenu.getSize().y,1);

    // Setting the window icon
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    gameMenu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    while (gameMenu.isOpen()){
        sf::Event event;
        while (gameMenu.pollEvent(event))
        {
            // Close the window
            if (event.type == sf::Event::Closed){
                gameMenu.close();
            }

            // Check for key events
            if (event.type == sf::Event::KeyReleased){
                // Treat all valid key events for GUI interactions
                switch (event.key.code){
                // Move Up
                case sf::Keyboard::Up:
                case sf::Keyboard::W:
                    mainMenu.MoveUp(4);
                    break;
                
                // Move Down
                case sf::Keyboard::Down:
                case sf::Keyboard::S:
                    mainMenu.MoveDown(4);
                    break;

                // Select option
                case sf::Keyboard::Return:
                case sf::Keyboard::Space:
                    // Building other windows
                    switchWindow(mainMenu.InterfacePressed(), gameMenu);
                    break;

                default:
                    break;
                }
              
            }
        }

        gameMenu.clear(sf::Color(49,21,58));
        mainMenu.draw(gameMenu);
        gameMenu.display();
    }


    return 0;
}
