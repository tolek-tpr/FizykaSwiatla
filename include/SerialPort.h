//
// Created by tolek on 11/6/26.
//

#ifndef TRAFFICCONTROLLER_SERIALPORT_H
#define TRAFFICCONTROLLER_SERIALPORT_H

#include <string>
#include <libserialport.h>
#include <vector>

class SerialPort {
public:
    SerialPort(const char* port_name);
    ~SerialPort();

    void write(const std::string &message) const;

    bool open() {
        return port != nullptr;
    }

    std::string readAvailable() const;

    // Returns complete newline-terminated messages.
    std::vector<std::string> readMessages();
private:
    sp_port* port = nullptr;
    std::string receiveBuffer;
};

#endif //TRAFFICCONTROLLER_SERIALPORT_H
