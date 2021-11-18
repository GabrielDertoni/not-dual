#include <bitset>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "includes/spmc.hpp"
#include "includes/input.hpp"

namespace input {
    static std::bitset<sf::Keyboard::KeyCount> keysPressed;

    void registerKeyPress(sf::Event::KeyEvent key) {
        keysPressed.set(key.code);
    }

    void registerKeyRelease(sf::Event::KeyEvent key) {
        keysPressed.reset(key.code);
    }

    bool isKeyPressed(sf::Keyboard::Key keyCode) {
        return keysPressed[keyCode];
    }
}

