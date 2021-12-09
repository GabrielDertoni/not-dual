#ifndef _SUPERPOWER_H_
#define _SUPERPOWER_H_

#include <memory>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"
#include "includes/renderer.hpp"
#include "includes/rendering.hpp"
#include "includes/time.hpp"

#define POWER_SIZE 10
#define SUPER_POWER_INTERVAL      (std::chrono::seconds(1))
#define SUPER_POWER_INTERVAL_LIFE (std::chrono::seconds(5))

extern Timestamp globalCounter;

class SuperPower: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    SuperPower(Timestamp created);
    Timestamp getLastPower();

private:
    Timestamp lastPower;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

class Spawner: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    Spawner(Timestamp created);

private:
    Timestamp lastPower;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
