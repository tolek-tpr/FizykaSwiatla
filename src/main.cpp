#include <SerialPort.h>
#include <chrono>
#include <cstring>
#include <Packet.h>
#include <thread>
#include <fstream>
#include <iostream>
#include <string>

#include "Intersection.h"

using namespace std::chrono_literals;

constexpr const char* FALLBACK_PATH = "kostki.txt";

#ifdef _WIN32
constexpr const char* FALLBACK_PORT = "COM3";
#else
constexpr const char* FALLBACK_PORT = "/dev/ttyACM0";
#endif

SerialPort *port;

int main(int argc, char** argv) {
    const char* PORT = FALLBACK_PORT;
    const char* PATH = FALLBACK_PATH;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--port" || arg == "-p") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for " << arg << '\n';
                return 1;
            }

            PORT = argv[++i];

        } else if (arg == "--file" || arg == "-f") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for " << arg << '\n';
                return 1;
            }

            PATH = argv[++i];

        } else {
            std::cerr << "Unknown argument: " << arg << '\n';
            return 1;
        }
    }

    std::cout << "Using port: " << PORT << "\n";
    std::cout << "Using file: " << PATH << "\n";

    port = new SerialPort(PORT);
    std::vector<Intersection*> intersections;

    while (!port->open()) {
        std::this_thread::sleep_for(10ms);
        port = new SerialPort(PORT);
    }

    std::ifstream ifs(PATH);
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

    while (true) {
        for (const auto& message : port->readMessages()) {
            //std::cout << "Received: " << message << '\n';

            // Parse JSON or your packet format here.
        }
        for (auto& is : intersections) {
             if (is->update()) {
                 //std::cout << "Updating intersection: " << is->getMac() << std::endl;
                 packet_send(port, is);
                 std::this_thread::sleep_for(3ms);
             }
        }

        std::this_thread::sleep_for(2ms);
    }

    return 0;
}
