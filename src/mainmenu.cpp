#include "includes/mainmenu.hpp"

// int screenType → 1: mainMenu, 2: gameOverScreen
MainMenu::MainMenu(float widht, float height, int screenType){

    std::string titleText, opt1, opt2;
    // Making adjustments according to the screen we're building
    if(screenType == 1){// Main Menu
        titleText = "NOT-DUAL";
        opt1 = "Let's play!";
        opt2 = "How to play?";

    }
    if(screenType == 2){// Game Over
        titleText = "GAME OVER";
        opt1 = "Play again!";
        opt2 = "Back to menu";

    }

    // Loading and applying the background texture to a sprite
    texture.loadFromFile("resources/bg_star.png");
    texture.setRepeated(true);

    if (!titleFont.loadFromFile("resources/fonts/vermin_vibes_1989.ttf")){
        std::cout << "Font not found.";
    }

    //Title
    windowTitle.setFont(titleFont);
    windowTitle.setFillColor(sf::Color::White);
    windowTitle.setString(titleText);
    windowTitle.setCharacterSize(90);
    windowTitle.setOrigin(windowTitle.getLocalBounds().left + windowTitle.getLocalBounds().width / 2.0f, 0);
    windowTitle.setPosition(WIDTH/2.0f, 50);

    if (!font.loadFromFile("resources/fonts/BitPotion.ttf")){
        std::cout << "Font not found.";
    }


    //Main Button
    menuOptions[0].setFont(font);
    menuOptions[0].setFillColor(sf::Color::White);
    menuOptions[0].setOutlineThickness(1.5f);
    menuOptions[0].setOutlineColor(sf::Color::White);
    menuOptions[0].setString(opt1);
    menuOptions[0].setCharacterSize(70);
    menuOptions[0].setOrigin(menuOptions[0].getLocalBounds().left + menuOptions[0].getLocalBounds().width / 2.0f, 0);
    menuOptions[0].setPosition(WIDTH/2.0f, 180);


    //About
    menuOptions[1].setFont(font);
    menuOptions[1].setFillColor(sf::Color(49,21,58));
    menuOptions[1].setOutlineThickness(1.5f);
    menuOptions[1].setOutlineColor(sf::Color::White);
    menuOptions[1].setString(opt2);
    menuOptions[1].setCharacterSize(70);
    menuOptions[1].setOrigin(menuOptions[1].getLocalBounds().left + menuOptions[1].getLocalBounds().width / 2.0f, 0);
    menuOptions[1].setPosition(WIDTH/2.0f, 230);


    //Quit game
    menuOptions[2].setFont(font);
    menuOptions[2].setFillColor(sf::Color(49,21,58));
    menuOptions[2].setOutlineThickness(1.5f);
    menuOptions[2].setOutlineColor(sf::Color::White);
    menuOptions[2].setString("Quit game");
    menuOptions[2].setCharacterSize(70);
    menuOptions[2].setOrigin(menuOptions[2].getLocalBounds().left + menuOptions[2].getLocalBounds().width / 2.0f, 0);
    menuOptions[2].setPosition(WIDTH/2.0f, 280);


    // Setting the indicator
    optionChosen.setFont(font);
    optionChosen.setFillColor(sf::Color::White);
    optionChosen.setString(">");
    optionChosen.setOutlineThickness(1.5f);
    optionChosen.setOutlineColor(sf::Color::White);
    optionChosen.setCharacterSize(70);
    optionChosen.setOrigin(optionChosen.getLocalBounds().left + optionChosen.getLocalBounds().width / 2.0f, 0);
    optionChosen.setPosition(160, menuOptions[0].getPosition().y);


    interfaceSelected = 0;
}

//Draw MainMenu
void MainMenu::draw(sf::RenderWindow& window){

    sf::Sprite bg(texture);
    bg.setColor(sf::Color(255, 255, 255));
    window.draw(bg);

    for (int i = 0; i < MAX_NUM_OPTIONS; ++i) {
        window.draw(menuOptions[i]);
    }

    window.draw(optionChosen);
    window.draw(windowTitle);
}

//Set the properties of a Render::Text
/*Text MainMenu::textFormat(String fontName, String s, float outline, int characterSize, float posX, float posY){
    Font font;
    if (!font.loadFromFile("resources/fonts/"+fontName)){
        cout << "Font not found.";
    }

    Text text;

    text.setFont(font);
    text.setFillColor(Color::White);
    text.setString(s);
    text.setOutlineThickness(outline);
    text.setOutlineColor(Color(175,202,249));
    text.setCharacterSize(characterSize);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, 0);
    text.setPosition(posX, posY);

    return text;
}*/

//MoveUp
void MainMenu::MoveUp(int max){

    if (interfaceSelected - 1 >= 0) {
        menuOptions[interfaceSelected].setFillColor(sf::Color(49,21,58));
        menuOptions[interfaceSelected].setOutlineColor(sf::Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(1.5f);

        interfaceSelected--;
        if (interfaceSelected == -1) {
            interfaceSelected = max;
        }

        optionChosen.setPosition(160, menuOptions[interfaceSelected].getPosition().y);
        menuOptions[interfaceSelected].setFillColor(sf::Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(1.5f);

    }
}

//MoveDown
void MainMenu::MoveDown(int max){

    if (interfaceSelected + 1 <= max) {
        menuOptions[interfaceSelected].setFillColor(sf::Color(49,21,58));
        menuOptions[interfaceSelected].setOutlineColor(sf::Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(1.5f);


        interfaceSelected++;
        if (interfaceSelected == max) {
            interfaceSelected = 0;
        }

                optionChosen.setPosition(160, menuOptions[interfaceSelected].getPosition().y);
        menuOptions[interfaceSelected].setFillColor(sf::Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(1.5f);
    }
}
