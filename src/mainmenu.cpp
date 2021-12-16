#include "includes/mainmenu.hpp"

using namespace std;

MainMenu::MainMenu(float widht, float height){
    // Loading and applying the background texture to a sprite
    texture.loadFromFile("resources/bg_star.png");
    texture.setRepeated(true);

    if (titleFont.loadFromFile("resources/fonts/vermin_vibes_1989.ttf")){
        cout << "Font not found.";
    }

    // Title
    //windowTitle = textFormat("vermin_vibes_1989.ttf", "NOTDUAL", 0.0f, 90, WIDTH/2.0f, 0.0f);
    
    //Title
    windowTitle.setFont(titleFont);
    windowTitle.setFillColor(Color::White);
    windowTitle.setString("NOTDUAL");
    windowTitle.setCharacterSize(90);
    windowTitle.setOrigin(windowTitle.getLocalBounds().left + windowTitle.getLocalBounds().width / 2.0f, 0);
    windowTitle.setPosition(WIDTH/2.0f, 0.0f);

    if (!font.loadFromFile("resources/fonts/BitPotion.ttf")){
        cout << "Font not found.";
    }

    //Main Button
    menuOptions[0].setFont(font);
    menuOptions[0].setFillColor(Color::White);
    menuOptions[0].setString("Let's play!");
    menuOptions[0].setCharacterSize(70);
    menuOptions[0].setOrigin(menuOptions[0].getLocalBounds().left + menuOptions[0].getLocalBounds().width / 2.0f, 0);
    menuOptions[0].setPosition(WIDTH/2.0f, 130);
    menuOptions[0].setOutlineThickness(5.0f);
    menuOptions[0].setOutlineColor(Color(185,192,203));

    //About
    menuOptions[1].setFont(font);
    menuOptions[1].setFillColor(Color::White);
    menuOptions[1].setString("About us");
    menuOptions[1].setCharacterSize(70);
    menuOptions[1].setOrigin(menuOptions[1].getLocalBounds().left + menuOptions[1].getLocalBounds().width / 2.0f, 0);
    menuOptions[1].setPosition(WIDTH/2.0f, 180);

    //Quit game
    menuOptions[2].setFont(font);
    menuOptions[2].setFillColor(Color::White);
    menuOptions[2].setString("Quit game");
    menuOptions[2].setCharacterSize(70);
    menuOptions[2].setOrigin(menuOptions[2].getLocalBounds().left + menuOptions[2].getLocalBounds().width / 2.0f, 0);
    menuOptions[2].setPosition(WIDTH/2.0f, 230);

    // Setting the indicator
    optionChosen.setFont(font);
    optionChosen.setFillColor(Color::White);
    optionChosen.setString(">");
    optionChosen.setOutlineThickness(5.0f);
    optionChosen.setOutlineColor(Color(185,192,203));
    optionChosen.setCharacterSize(70);
    optionChosen.setOrigin(optionChosen.getLocalBounds().left + optionChosen.getLocalBounds().width / 2.0f, 0);
    optionChosen.setPosition(180, menuOptions[0].getPosition().y);


    interfaceSelected = 0;    
}

MainMenu::~MainMenu(){
}

//Draw MainMenu
void MainMenu::draw(RenderWindow& window){

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
Text MainMenu::textFormat(String fontName, String s, float outline, int characterSize, float posX, float posY){
    Font font;
    if (!font.loadFromFile("resources/fonts/"+fontName)){
        cout << "Font not found.";
    }

    Text text;

    text.setFont(font);
    text.setFillColor(Color::White);
    text.setString(s);
    text.setOutlineThickness(outline);
    text.setOutlineColor(Color(185,192,203));
    text.setCharacterSize(characterSize);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, 0);
    text.setPosition(posX, posY);

    return text;
}

//MoveUp
void MainMenu::MoveUp(){

    if (interfaceSelected - 1 >= 0) {
        menuOptions[interfaceSelected].setFillColor(Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(0.0f);

        interfaceSelected--;
        if (interfaceSelected == -1) {
            interfaceSelected = MAX_NUM_OPTIONS;
        }
        
        optionChosen.setPosition(180, menuOptions[interfaceSelected].getPosition().y);
        menuOptions[interfaceSelected].setOutlineThickness(5.0f);
        menuOptions[interfaceSelected].setOutlineColor(Color(185,192,203));

    }
}

//MoveDown
void MainMenu::MoveDown(){

    if (interfaceSelected + 1 <= MAX_NUM_OPTIONS) {
        menuOptions[interfaceSelected].setFillColor(Color::White);
        menuOptions[interfaceSelected].setOutlineThickness(0.0f);

        interfaceSelected++;
        if (interfaceSelected == MAX_NUM_OPTIONS) {
            interfaceSelected = 0;
        }

                optionChosen.setPosition(180, menuOptions[interfaceSelected].getPosition().y);
        menuOptions[interfaceSelected].setOutlineThickness(5.0f);
        menuOptions[interfaceSelected].setOutlineColor(Color(185,192,203));
        
    }
}