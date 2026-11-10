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
        Off = 4,
        Red = 2,
        Yellow = 3,
        Green = 1
    };

    enum class Phase {
        CarsGreen,
        CarsYellow,
        AllRedBeforePedestrians,
        PedestriansGreen,
        PedestriansFlashing,
        AllRedBeforeCars,
        CarsRedYellow
    };

    struct State {
        Color car;
        Color ped1;
        Color ped2;

        bool operator==(const State&) const = default;
    };

    explicit Intersection(std::string mac, uint32_t offset);

    // Advances the simulation.
    // Returns true if the output state changed.
    bool update();

    State getState() const;
    Phase getPhase() const;

    std::string getMac() const;

private:
    uint32_t offset;

    Phase phase = Phase::CarsGreen;

    State currentState{};

    Clock::time_point phaseStarted;

    void transition();
    State calculateState() const;

    std::chrono::milliseconds getDuration() const;

    std::string mac;
};

#endif //TRAFFICCONTROLLER_INTERSECTION_H
