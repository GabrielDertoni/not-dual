/*
#ifndef _INPUT_H_
#define _INPUT_H_

#include <thread>
#include <memory>

#include <SFML/Window/Event.hpp>

#include "includes/spmc.h"

namespace input {
    class Broadcaster {
    public:
        Broadcaster(sf::RenderWindow& window);

        spmc::Receiver<sf::Event> getEventReceiver();

    private:
        void eventLoop();

        sf::RenderWindow& mWindow;
        std::unique_ptr<spmc::Sender<sf::Event>> mSender;
        std::thread mEventThread;
    };
}

#endif
*/
