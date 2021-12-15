#include <iostream>
#include <bitset>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "includes/spmc.hpp"
#include "includes/input.hpp"

namespace input {
    static std::bitset<sf::Keyboard::KeyCount> keysPressed;

    void registerKeyPress(sf::Event::KeyEvent key) {
        if (key.code >= sf::Keyboard::KeyCount || key.code < 0) return;
        keysPressed.set(key.code);
    }

    void registerKeyRelease(sf::Event::KeyEvent key) {
        if (key.code >= sf::Keyboard::KeyCount || key.code < 0) return;
        keysPressed.reset(key.code);
    }

    bool isKeyPressed(sf::Keyboard::Key keyCode) {
        if (keyCode >= sf::Keyboard::KeyCount || keyCode < 0) return false;
        return keysPressed[keyCode];
    }
}

