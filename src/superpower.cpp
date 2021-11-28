<<<<<<< HEAD
#include <iostream>
#include <chrono>
#include <ranges>

=======
#include <chrono>
#include <ranges>

//#include "includes/superpower.hpp"
//#include "includes/time.hpp"
//#include "includes/collider.hpp"
//#include "includes/rigidbody.hpp"

>>>>>>> fccf449284f37257d5ccab9e5b032895a7cd98a2
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include "includes/superpower.hpp"
#include "includes/gameobj.hpp"
#include "includes/settings.hpp"

SuperPower::SuperPower(
        Timestamp lastPower
) :
    lastPower(lastPower)
{}

std::unique_ptr<Component> SuperPower::clone() {
    return std::make_unique<SuperPower>(*this);
}

void SuperPower::initialize(GameObject& gameObject) {}

void SuperPower::update(GameObject& gameObject) {
    Timestamp now = getNow();

    if (now - lastPower <= std::chrono::milliseconds(0)) {
<<<<<<< HEAD
        std::cout << "AQUI" << std::endl;
=======
>>>>>>> fccf449284f37257d5ccab9e5b032895a7cd98a2
        gameObject.destroy();
    }
}

