#include "mymodbus.h"

MyModbus::MyModbus(QObject* parent)
    : QSerialPort(parent) {
}

MyModbus::Error MyModbus::error() const { return m_error; }

uint8_t MyModbus::address() const { return m_address; }

void MyModbus::setAddress(uint8_t newAddress) { m_address = newAddress; }
