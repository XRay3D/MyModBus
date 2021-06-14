#include "libmodbus.h"
#include "modbus.h"
#include <QSerialPort>

LibModbus::LibModbus(QObject* parent)
    : QObject(parent) {
}

LibModbus::~LibModbus() { close(); }

bool LibModbus::open() {
    if(mb)
        close();
    mb = modbus_new_rtu(("\\\\.\\" + m_portName.toLocal8Bit()).data(), m_baudRate, m_parity, m_dataBits, m_stopBits);
    modbus_set_slave(mb, 1);
    //modbus_set_debug(mb, true);
    m_connected = modbus_connect(mb) == 0;
    return m_connected;
}

void LibModbus::close() {
    if(!mb)
        return;
    modbus_close(mb);
    modbus_free(mb);
    mb = nullptr;
    m_connected = false;
}

QString LibModbus::errorString() const {
    return QString("Connection failed: %1").arg(modbus_strerror(errno));
}

int LibModbus::modbusReadRegisters(int addr, int nb, uint16_t* dest) const {
    return modbus_read_registers(mb, addr, nb, dest);
}

int LibModbus::modbusWriteRegisters(int addr, int nb, uint16_t* dest) const {
    return modbus_write_registers(mb, addr, nb, dest);
}

QString LibModbus::portName() const { return m_portName; }
void LibModbus::setPortName(const QString& portName) {
    close();
    m_portName = portName;
    open();
}

qint32 LibModbus::baudRate() const { return m_baudRate; }
bool LibModbus::setBaudRate(qint32 baudRate) {
    bool connected = m_connected;
    if(m_baudRate != baudRate) {
        close();
        m_baudRate = baudRate;
        emit baudRateChanged(baudRate);
    }
    return connected ? open() : true;
}

LibModbus::DataBits LibModbus::dataBits() const { return m_dataBits; }
bool LibModbus::setDataBits(DataBits dataBits) {
    bool connected = m_connected;
    if(m_dataBits != dataBits) {
        close();
        m_dataBits = dataBits;
        emit dataBitsChanged(dataBits);
    }
    return connected ? open() : true;
}

LibModbus::Parity LibModbus::parity() const { return m_parity; }
bool LibModbus::setParity(Parity parity) {
    bool connected = m_connected;
    if(m_parity != parity) {
        close();
        m_parity = parity;
        emit parityChanged(parity);
    }
    return connected ? open() : true;
}

LibModbus::StopBits LibModbus::stopBits() const { return m_stopBits; }
bool LibModbus::setStopBits(StopBits stopBits) {
    bool connected = m_connected;
    if(m_stopBits != stopBits) {
        close();
        m_stopBits = stopBits;
        emit stopBitsChanged(stopBits);
    }
    return connected ? open() : true;
}
