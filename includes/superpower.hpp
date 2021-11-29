#ifndef _SUPERPOWER_H_
#define _SUPERPOWER_H_

#include <memory>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"
#include "includes/spaceship.hpp"
#include "includes/time.hpp"

#define POWER_SIZE 10
#define SUPER_POWER_INTERVAL      (std::chrono::milliseconds(20000))
#define SUPER_POWER_INTERVAL_LIFE (std::chrono::milliseconds(10000))

extern Timestamp globalCounter;

class SuperPower: public Behaviour {
public:
    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
    virtual std::unique_ptr<Component> clone();

    SuperPower(Timestamp created);
    Timestamp getLastPower();

private:
    Timestamp lastPower;
};

#endif
