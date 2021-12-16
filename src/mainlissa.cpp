#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>
#include <list>

#include <SFML/Graphics.hpp>

#include "includes/mainmenu.hpp"

using namespace sf;

void switchWindow(int x, RenderWindow& gameMenu);

int mainlissa(){

    // Building the Main window
    sf::RenderWindow gameMenu(VideoMode(WIDTH, HEIGHT), "NOTDUAL", Style::Default);
    MainMenu mainMenu(gameMenu.getSize().x, gameMenu.getSize().y);

    // Setting the window icon
    sf::Image icon;
    icon.loadFromFile("resources/icon.png");
    gameMenu.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());



    while (gameMenu.isOpen()){
        Event event;
        while (gameMenu.pollEvent(event))
        {
            // Close the window
            if (event.type == Event::Closed){
                gameMenu.close();
            }

            // Check for key events
            if (event.type == Event::KeyReleased){
                // Treat all valid key events for GUI interactions
                switch (event.key.code){
                // Move Up
                case Keyboard::Up:
                case Keyboard::W:
                    mainMenu.MoveUp();
                    break;
                
                // Move Down
                case Keyboard::Down:
                case Keyboard::S:
                    mainMenu.MoveDown();
                    break;

                // Select option
                case Keyboard::Return:
                case Keyboard::Space:
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

void switchWindow(int x, RenderWindow& gameMenu){
    
    sf::RenderWindow playGame(VideoMode(WIDTH, HEIGHT), "Let's play!");
    sf::RenderWindow aboutSection(VideoMode(WIDTH, HEIGHT), "aboutSection");

    switch (x)
    {

        // Switch to playGame
        case 0:
            while (playGame.isOpen()){
                Event aEvent;
                while (playGame.pollEvent(aEvent)){
                    if (aEvent.type == Event::Closed){
                        playGame.close();
                    }

                    if (aEvent.type == Event::KeyPressed){
                        if (aEvent.key.code == Keyboard::Escape){
                            playGame.close();
                        }
                    }   

                }

                aboutSection.close();
                playGame.clear(sf::Color(49,21,58));
                playGame.display();
            }
            break;
        
        // Switch to aboutSection
        case 1:
            while (aboutSection.isOpen())
            {
                Event aEvent;
                while (aboutSection.pollEvent(aEvent)){
                    if (aEvent.type == Event::Closed){
                        aboutSection.close();
                    }
                    if (aEvent.type == Event::KeyPressed){
                        if (aEvent.key.code == Keyboard::Escape){
                            aboutSection.close();
                        }
                    }   

                }

                playGame.close();
                aboutSection.clear(sf::Color(49,21,58));
                aboutSection.display();
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