#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>

#define WIDTH  640
#define HEIGHT 480

#define BG_PATH "./resources/bg_star.png"
#define RS_PATH "./resources/rSpaceship.png"
#define LS_PATH "./resources/lSpaceship.png"
#define NB_PATH "./resources/normalBullet.png"
#define SB_PATH "./resources/superBullet.png"
#define P_PATH "./resources/particle.png"

#define PLAYER_SIZE 32

static const sf::Vector2f playerSize(PLAYER_SIZE, PLAYER_SIZE);

#endif
