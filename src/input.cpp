/*
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "includes/spmc.h"
#include "includes/input.h"

using namespace input;

Broadcaster::Broadcaster(sf::RenderWindow& window) : window(window) {
    auto pair = spmc::channel<sf::Event>();
    mSender = std::make_unique(pair.first);
    spmc::Receiver<sf::Event> rx = pair.second;
    mEventThread = std::thread(&Broadcaster::eventLoop, this);
}

void Broadcaster::eventLoop() {
    sf::Event event;
    while (window.waitEvent(event)) {
        
    }
}
*/
