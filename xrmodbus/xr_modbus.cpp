#include "xr_modbus.h"

/* Table of CRC values for high-order byte */
static constexpr uint8_t tableCrcHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
    0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
    0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
    0x40
};

/* Table of CRC values for low-order byte */
static constexpr uint8_t tableCrcLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
    0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
    0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
    0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
    0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
    0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
    0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
    0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
    0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
    0x40
};

int id = qRegisterMetaType<ByteVector>("ByteVector");

class SerialPort : public QSerialPort {
    Modbus* const modBus;

public:
    SerialPort(Modbus* modBus)
        : modBus { modBus }
    {
        setPortName(0 ? "COM10" : "COM8");
        setBaudRate(Baud115200);
        setParity(NoParity);
        setDataBits(Data8);
        setStopBits(OneStop);
        setFlowControl(NoFlowControl);
        connect(this, &QIODevice::readyRead, this, &SerialPort::readyReadSlot);
        qDebug() << "open" << open(QIODevice::ReadWrite);
        //        startTimer(10);
    }
    virtual ~SerialPort() { }

    void readyReadSlot()
    {
        auto available = bytesAvailable();
        if (!available)
            return;
        auto lastSize = modBus->response.size();

        modBus->response.resize(modBus->response.size() + available);
        read(reinterpret_cast<char*>(modBus->response.data() + lastSize), available);
        modBus->semaphore.release(available);
        qDebug() << "readyRead" << lastSize << available << toHex(modBus->response);
    }

    void openSlot() { open(QIODevice::ReadWrite); }

    void write(const ByteVector& data)
    {
        QSerialPort::write(reinterpret_cast<const char*>(data.data()), data.size());
    }

    // QObject interface
protected:
    void timerEvent(QTimerEvent* event) override { readyReadSlot(); }
};

Modbus::Modbus(QObject* parent)
    : Object(parent)
    , m_port { new SerialPort { this } }
{
    m_port->moveToThread(&portThread);
    connect(&portThread, &QThread::finished, m_port, &QObject::deleteLater);

    connect(this,
        &Modbus::write,
        static_cast<SerialPort*>(m_port),
        qOverload<const ByteVector&>(&SerialPort::write));
    connect(this, &Modbus::close, static_cast<SerialPort*>(m_port), &QSerialPort::close);
    connect(this, &Modbus::open, static_cast<SerialPort*>(m_port), &SerialPort::openSlot);

    portThread.start(QThread::NormalPriority);
}

Modbus::~Modbus()
{
    portThread.quit();
    portThread.wait();
}

void Modbus::prepare()
{
    request.clear();
    response.clear();
    m_errorString.clear();
    semaphore.acquire(semaphore.available());
    m_port->readAll();
}

Modbus::Error Modbus::error() const
{
    return m_error;
}

uint16_t Modbus::crc16(std::span<uint8_t> data)
{
    union {
        uint16_t crc = 0xFFFF;
        struct {
            uint8_t lo; /* low CRC byte initialized */
            uint8_t hi; /* high CRC byte initialized */
        };
    } u;
    for (auto&& byte : data) {
        auto i = u.hi ^ static_cast<uint8_t>(byte);
        u.hi = u.lo ^ tableCrcHi[i];
        u.lo = tableCrcLo[i];
    }
    return u.crc;
}

uint8_t Modbus::address() const
{
    return m_address;
}

void Modbus::setAddress(uint8_t newAddress)
{
    m_address = newAddress;
}

int Modbus::writeRequest()
{
    m_port->readAll();
    logRequest();
    emit write(request);
    return {}; //m_port->write(reinterpret_cast<const char*>(request.data()), request.size());
}

bool Modbus::readAndCheck()
{
    if (semaphore.tryAcquire(5, m_timeout)
        && response[0] == uint8_t(m_address)
        && bool(response[1] & uint8_t { 0x80 })) {
        m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
        qDebug() << "err response" << toHex(response).mid(0, 10 /*5 bytes only*/);
        return {};
    }
    return true;
}

int Modbus::readResponse(size_t count) { return semaphore.tryAcquire(count, m_timeout); }

bool Modbus::checkCrc()
{
    uint16_t crc1 = request.crc();
    uint16_t crc2 = crc16({ request.data(), static_cast<size_t>(request.size() - 2) });
    crc1 = ByteOrder::Swap2B()(crc1);
    const bool ok = crc1 == crc2;
    m_error = ok ? NoError : CrcError;
    //qDebug() << m_error << crc1 << crc2;
    return ok;
}
