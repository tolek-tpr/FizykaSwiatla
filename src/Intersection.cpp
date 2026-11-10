//
// Created by tolek on 11/6/26.
//
#include <Intersection.h>

#include <utility>

Intersection::Intersection(std::string mac, const uint32_t offset) : offset(offset), phaseStarted(Clock::now()), mac(std::move(mac)) {
    auto remaining = std::chrono::milliseconds(-offset);

    while (remaining >= getDuration()) {
        remaining -= getDuration();
        transition();
    }

    phaseStarted -= remaining;
    currentState = calculateState();
}

bool Intersection::update() {
    auto now = Clock::now();

    while (now - phaseStarted >= getDuration()) {
        phaseStarted += getDuration();
        transition();
    }

    State next = calculateState();

    if (next == currentState)
        return false;

    currentState = next;
    return true;
}

void Intersection::transition() {
    switch (phase) {
        case Phase::CarsGreen:
            phase = Phase::CarsYellow;
            break;

        case Phase::CarsYellow:
            phase = Phase::AllRedBeforePedestrians;
            break;

        case Phase::AllRedBeforePedestrians:
            phase = Phase::PedestriansGreen;
            break;

        case Phase::PedestriansGreen:
            phase = Phase::PedestriansFlashing;
            break;

        case Phase::PedestriansFlashing:
            phase = Phase::AllRedBeforeCars;
            break;

        case Phase::AllRedBeforeCars:
            phase = Phase::CarsRedYellow;
            break;

        case Phase::CarsRedYellow:
            phase = Phase::CarsGreen;
            break;
    }
}

std::chrono::milliseconds Intersection::getDuration() const {
    switch (phase) {
        case Phase::CarsGreen:                 return std::chrono::milliseconds(10000);
        case Phase::CarsYellow:                return std::chrono::milliseconds(5000);
        case Phase::AllRedBeforePedestrians:   return std::chrono::milliseconds(5000);
        case Phase::PedestriansGreen:          return std::chrono::milliseconds(10000);
        case Phase::PedestriansFlashing:       return std::chrono::milliseconds(6000);
        case Phase::AllRedBeforeCars:          return std::chrono::milliseconds(5000);
        case Phase::CarsRedYellow:             return std::chrono::milliseconds(5000);
    }

    return std::chrono::milliseconds(1000);
}

Intersection::State Intersection::calculateState() const {
    switch (phase) {
        case Phase::CarsGreen:
            return {Color::Green, Color::Red, Color::Red};

        case Phase::CarsYellow:
            return {Color::Yellow, Color::Red, Color::Red};

        case Phase::AllRedBeforePedestrians:
            return {Color::Red, Color::Red, Color::Red};

        case Phase::PedestriansGreen:
            return {Color::Red, Color::Green, Color::Green};

        case Phase::PedestriansFlashing: {
            // Toggle pedestrian lights every 500ms.
            auto elapsed = duration_cast<std::chrono::milliseconds>(
                Clock::now() - phaseStarted
            );

            bool on = (elapsed.count() / 500) % 2 == 0;

            return {
                Color::Red,
                on ? Color::Green : Color::Off,
                on ? Color::Green : Color::Off
            };
        }

        case Phase::AllRedBeforeCars:
            return {Color::Red, Color::Red, Color::Red};

        case Phase::CarsRedYellow:
            return {Color::Yellow, Color::Red, Color::Red};
    }

    return {Color::Red, Color::Red, Color::Red};
}

Intersection::State Intersection::getState() const {
    return currentState;
}

Intersection::Phase Intersection::getPhase() const {
    return phase;
}

std::string Intersection::getMac() const {
    return mac;
}