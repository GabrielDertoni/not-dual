#ifndef _SUPERPOWER_H_
#define _SUPERPOWER_H_

#include <memory>
#include <optional>

#include "includes/gameobj.hpp"
#include "includes/collider.hpp"
#include "includes/renderer.hpp"
#include "includes/rendering.hpp"
#include "includes/time.hpp"

#define POWER_SIZE 10
#define SUPER_POWER_INTERVAL      (std::chrono::seconds(7))
#define SUPER_POWER_INTERVAL_LIFE (std::chrono::seconds(5))

class SuperPower: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    SuperPower() = default;

private:
    // This is an option because the value may not be initialized before
    // `initialize` is called.
    std::optional<Timestamp> created;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

class PowerSpawner: public Behaviour {
public:
    DERIVE_CLONE_COMPONENT

    PowerSpawner() = default;

private:
    // This is an option because the value may not be initialized before
    // `initialize` is called.
    std::optional<Timestamp> lastPower;

    virtual void initialize(GameObject& gameObject);
    virtual void update(GameObject& gameObject);
};

#endif
