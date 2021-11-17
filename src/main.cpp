#include <iostream>
#include <semaphore>

#include <SFML/Graphics.hpp>

#include "includes/player.h"
#include "includes/spmc.h"

std::binary_semaphore gameLoop(0);
std::binary_semaphore gameLoopDone(1);
std::binary_semaphore drawThread(0);
std::binary_semaphore drawThreadDone(1);

Player p1(sf::Vector2f(100, 200), sf::Color::Green);
Player p2(sf::Vector2f(500, 200), sf::Color::Red);

std::deque<sf::Drawable*> drawQueue;

bool done;

void gameThread(spmc::Receiver<sf::Event> eventSender) {
    p1.setRotation(90);
    p2.setRotation(270);

    while (!done) {
        gameLoop.acquire();

        p1.update();
        p2.update();

        gameLoopDone.release();
    }
}


void renderThread(sf::RenderWindow& window, spmc::Sender<sf::Event> eventSender) {
    while (window.isOpen()) {
        drawThread.acquire();

        window.clear();
        while (!drawQueue.empty()) {
            sf::Drawable* shape = drawQueue.front();
            window.draw(*shape);
            drawQueue.pop_front();
        }
        window.display();

        drawThreadDone.release();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                done = true;
            }

            eventSender.send(event);
        }
    }
}

int main() {
    int width = 600;
    int height = 400;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!", sf::Style::Default, settings);

    sf::RectangleShape divisionLine(sf::Vector2f(1, height));
    divisionLine.setPosition(width / 2, 0);

    /*
    auto pair = spmc::channel();
    Sender<sf::Event> inputSender = pair.first;
    Receiver<sf::Event> rx1 = pair.second;
    Receiver<sf::Event> rx2 = pair.second;
    */

    done = false;

    while (!done) {
        // Game loop starts
        gameLoop.release();

        // Render stuff starts
        drawThread.release();

        // Game loop ends
        gameLoopDone.acquire();

        // Render thread from previous iteration ends.
        drawThreadDone.acquire();

        // Push stuff to the draw queue
        drawQueue.push_back(&p1.mesh);
        drawQueue.push_back(&p2.mesh);
        drawQueue.push_back(&divisionLine);

    }

    return 0;
}
