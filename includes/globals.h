#include <deque>
#include <array>

#include <SFML/Window/Keyboard.hpp>

extern std::deque<sf::Event> eventQueue;
extern std::array<bool, sf::Keyboard::KeyCount> keysPressed;
