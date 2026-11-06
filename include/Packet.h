//
// Created by tolek on 11/6/26.
//

#ifndef TRAFFICCONTROLLER_PACKET_H
#define TRAFFICCONTROLLER_PACKET_H

#include <SerialPort.h>
#include <Intersection.h>

static void packet_reset(SerialPort *port, const Intersection *intersection) {
    std::string data;
    data.append(intersection->getMac());

    data.append("00");

    for (int i = 0; i < 3; i++) {
        data[17] = static_cast<int8_t>('1' + i);
        data[18] = static_cast<int8_t>('0' + 4);
        port->write(data);
    }
}

static void packet_send(SerialPort *port, const Intersection *intersection) {
    packet_reset(port, intersection);

    std::string data;
    data.append(intersection->getMac());

    auto state = intersection->getState();

    data += '1';
    data += static_cast<int8_t>('0' + static_cast<int8_t>(state.ped1));

    port->write(data);

    data[17] = '2';
    data[18] = static_cast<int8_t>('0' + static_cast<int8_t> (state.ped2));

    port->write(data);

    data[17] = '3';
    data[18] = static_cast<int8_t>('0' + static_cast<int8_t> (state.car));
}

#endif //TRAFFICCONTROLLER_PACKET_H
