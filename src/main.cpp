#include <iostream>
#include <SFML/Graphics.hpp>

#include "../includes/player.h"
#include "../includes/vec.h"

int main()
{
    int width = 600;
    int height = 400;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!", sf::Style::Default, settings);

    Player p1(sf::Vector2f(100, 200), sf::Color::Green);
    Player p2(sf::Vector2f(500, 200), sf::Color::Red);

    p1.setRotation(90);
    p2.setRotation(270);

    sf::RectangleShape divisionLine(sf::Vector2f(1, height));
    divisionLine.setPosition(width / 2, 0);

    while (window.isOpen())
    {
        window.clear();
        window.draw(p2);
        window.draw(p1);
        window.draw(divisionLine);
        window.display();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        p1.update();
        p2.update();
    }

    return 0;
}
