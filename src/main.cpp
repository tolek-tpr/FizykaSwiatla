#include <SerialPort.h>
#include <chrono>
#include <Packet.h>
#include <thread>
#include <fstream>
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
    std::vector<Intersection*> intersections;

    while (!port->open()) {
        std::this_thread::sleep_for(10ms);
        port = new SerialPort(PORT);
    }

    std::ifstream ifs("../data/kostki.txt");
    std::string line;
    while (std::getline(ifs, line)) {
        std::string mac = line.substr(0, line.find('|'));
        std::string offset = line.substr(line.find('|') + 1);
        intersections.push_back(new Intersection(mac, std::stoi(offset)));
    }

    for (auto& is : intersections) {
        packet_send(port, is);
        std::cout << "Resetting intersection: " << is->getMac() << std::endl;
        std::this_thread::sleep_for(3ms);
    }

    // return 0;

    while (true) {
        for (const auto& message : port->readMessages()) {
            //std::cout << "Received: " << message << '\n';

            // Parse JSON or your packet format here.
        }
        for (auto& is : intersections) {
             if (is->update()) {
                 packet_send(port, is);
                 std::this_thread::sleep_for(3ms);
             }
        }

        std::this_thread::sleep_for(2ms);
    }

    return 0;
}
