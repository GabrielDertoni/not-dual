
#include "includes/utils.hpp"

sf::Color colorLerp(sf::Color start, sf::Color end, float lerp) {
    return sf::Color(
        start.r + lerp * (end.r - start.r),
        start.g + lerp * (end.g - start.g),
        start.b + lerp * (end.b - start.b),
        start.a + lerp * (end.a - start.a)
    );
}
