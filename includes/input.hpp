#ifndef _INPUT_H_
#define _INPUT_H_

#include <thread>
#include <memory>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

namespace input {
    void registerKeyPress(sf::Event::KeyEvent key);
    void registerKeyRelease(sf::Event::KeyEvent key);

    bool isKeyPressed(sf::Keyboard::Key keyCode);
}

#endif
