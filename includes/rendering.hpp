#ifndef _RENDERING_H_
#define _RENDERING_H_

#include <deque>

#include <SFML/Graphics/Drawable.hpp>

extern std::deque<const sf::Drawable*> drawQueue;
extern std::deque<const sf::Drawable*> nextDrawQueue;

void setupDrawQueue();
void pushDrawable(const sf::Drawable* drawable);


#endif
