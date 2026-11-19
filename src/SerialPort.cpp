//
// Created by tolek on 11/6/26.
//

#include <SerialPort.h>
#include <iostream>
#include <libserialport.h>

#include <iomanip>
#include <string>
#include <thread>

using namespace std::chrono_literals;

void printHex(const std::string& str) {
    for (unsigned char c : str) {
        std::cout << std::hex
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(c) << ' ';
    }

    std::cout << std::dec << '\n';
}

SerialPort::SerialPort(const char* port_name) {
    if (sp_get_port_by_name(port_name, &port) != SP_OK) {
        std::cerr << "Failed to get serial port by name\n";
        return;
    }

    auto status = sp_open(port, SP_MODE_READ_WRITE);
    if (status != SP_OK) {
        std::cerr << "Failed to open serial port\n";
        std::cerr << status << "\n";
        sp_free_port(port);
        port = nullptr;
        return;
    }

    sp_set_baudrate(port, 115200);
    sp_set_bits(port, 8);
    sp_set_parity(port, SP_PARITY_NONE);
    sp_set_stopbits(port, 1);
    sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
}

SerialPort::~SerialPort() {
    sp_close(port);
    sp_free_port(port);
}

void SerialPort::write(const std::string &data) const {
    //printHex(data);
    //sp_nonblocking_write(port, data.c_str(), data.length());
    int result = sp_blocking_write(
        port,
        data.data(),
        data.size(),
        1000
    );
    //std::this_thread::sleep_for(1ms);

    if (result < 0) {
        std::cerr << "Serial write failed: " << result << '\n';
    }
    else if (result != data.size()) {
        std::cerr << "Incomplete write: "
                  << result << "/" << data.size() << " bytes\n";
    }
}

std::string SerialPort::readAvailable() const {
    if (!port)
        return {};

    char buffer[256];

    int bytes = sp_nonblocking_read(
        port,
        buffer,
        sizeof(buffer)
    );

    if (bytes <= 0)
        return {};

    return std::string(buffer, bytes);
}

std::vector<std::string> SerialPort::readMessages() {
    receiveBuffer += readAvailable();

    std::vector<std::string> messages;

    size_t position;

    while ((position = receiveBuffer.find('\n')) != std::string::npos) {
        messages.push_back(receiveBuffer.substr(0, position));

        receiveBuffer.erase(0, position + 1);
    }

    return messages;
}
