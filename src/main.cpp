#include <SerialPort.h>
#include <chrono>
#include <Packet.h>
#include <thread>
#include <iostream>

#include "Intersection.h"

using namespace std::chrono_literals;

#ifdef _WIN32
constexpr const char* PORT = "COM3";
#else
constexpr const char* PORT = "/dev/ttyACM0";
#endif

SerialPort *port;

int main() {
    port = new SerialPort(PORT);

    if (!port->open()) {
        return 1;
    }

    Intersection *is = new Intersection("10:00:3B:DE:5E:68", 0);
    packet_send(port, is);

    while (true) {

        for (const auto& message : port->readMessages()) {
            std::cout << "Received: " << message << '\n';

            // Parse JSON or your packet format here.
        }
        //for (auto& intersection : intersections) {
            if (is->update()) {
                packet_reset(port, is);
                packet_send(port, is);
            }
        //}

        std::this_thread::sleep_for(2ms);
    }

    return 0;
}
