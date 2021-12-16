#ifndef _MAINMENU_H_
#define _MAINMENU_H_

#include <iostream>
#include <semaphore>
#include <thread>
#include <memory>
#include <chrono>
#include <deque>
#include <list>

#include <SFML/Graphics.hpp>

using namespace sf;

#define MAX_NUM_OPTIONS 3
#define WIDTH  640
#define HEIGHT 360

class MainMenu{
public:
    MainMenu(float width, float height);

    void draw(RenderWindow& window);
    
    Text textFormat(String fontName, String s, float outline, int characterSize, float posX, float posY);
    
    void MoveUp();
    void MoveDown();

    int InterfacePressed() {
        return interfaceSelected;
    }

    ~MainMenu();

private:
    Text optionChosen;
    sf::Texture texture;
    int interfaceSelected;
    Font font;
    Font titleFont;
    Text windowTitle;
    Text menuOptions[MAX_NUM_OPTIONS];
};

#endif