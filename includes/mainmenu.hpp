#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>
#include <list>
#include <string>

#include <SFML/Graphics.hpp>

#include "includes/settings.hpp"


#define MAX_NUM_OPTIONS 3

class MainMenu{
public:
    MainMenu(float width, float height, bool showGameOverText);

    void draw(sf::RenderWindow& window);

    //sf::Text textFormat(std::String fontName, std::String s, float outline, int characterSize, float posX, float posY);

    void MoveUp(int max);
    void MoveDown(int max);

    int InterfacePressed() {
        return interfaceSelected;
    }

private:
    sf::Text optionChosen;
    sf::Texture texture;
    int interfaceSelected;
    sf::Font font;
    sf::Font titleFont;
    sf::Text windowTitle;
    sf::Text menuOptions[MAX_NUM_OPTIONS];
};

#endif
