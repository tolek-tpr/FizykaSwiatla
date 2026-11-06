//
// Created by tolek on 11/6/26.
//

#ifndef TRAFFICCONTROLLER_INTERSECTION_H
#define TRAFFICCONTROLLER_INTERSECTION_H

#include <chrono>
#include <cstdint>
#include <string>

class Intersection {
public:
    using Clock = std::chrono::steady_clock;

    enum class Color : int8_t {
        Off,
        Red,
        Yellow,
        Green
    };

    enum class Phase {
        CarsGreen,
        CarsYellow,
        AllRedBeforePedestrians,
        PedestriansGreen,
        PedestriansFlashing,
        AllRedBeforeCars
    };

    struct State {
        Color car;
        Color ped1;
        Color ped2;

        bool operator==(const State&) const = default;
    };

    explicit Intersection(std::string mac, uint8_t id);

    // Advances the simulation.
    // Returns true if the output state changed.
    bool update();

    State getState() const;
    Phase getPhase() const;

    uint8_t getId() const;

    std::string getMac() const;

private:
    uint8_t id;

    Phase phase = Phase::CarsGreen;

    State currentState{};

    Clock::time_point phaseStarted;

    void transition();
    State calculateState() const;

    std::chrono::milliseconds getDuration() const;

    std::string mac;
};

#endif //TRAFFICCONTROLLER_INTERSECTION_H
