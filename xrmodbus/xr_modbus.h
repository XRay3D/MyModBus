#pragma once

#include "../EnumHelper.h"
#include "../timer.h"
#include "xr_modbusutility.h"
#include <QApplication>
#include <QMutex>
#include <QMutexLocker>
#include <QSemaphore>
#include <QSerialPort>
#include <QThread>
#include <QTimer>
#include <span>

class Object : public QObject {
    Q_OBJECT
    friend class Modbus;
    Object(QObject* parent = nullptr)
        : QObject{parent} {
    }
signals:
    void open();
    void close();
    void write(const ByteVector& data);
};

inline auto toHex(const ByteVector& data) {
    return QByteArray(reinterpret_cast<const char*>(data.data()), data.size()).toHex('|').toUpper();
}

class Modbus : public Object {
    friend class SerialPort;

    Q_OBJECT

    template <typename T, size_t N>
    struct array_size {
        constexpr array_size(T (&)[N]) { }
        constexpr array_size(T (&&)[N]) { }
        constexpr operator size_t() { return N; }
    };
    template <typename T, size_t N>
    array_size(T (&)[N]) -> array_size<T, N>;
    template <typename T, size_t N>
    array_size(T (&&)[N]) -> array_size<T, N>;

public:
    explicit Modbus(QObject* parent = nullptr);
    ~Modbus();
    enum Error {
        // Text  Code // Details
        NoError = 0,
        // Function code received in the query is not recognized or allowed by slave
        IllegalFunction = 1,
        // Data address of some or all the required entities are not allowed or do not exist in slave
        IllegalDataAddress = 2,
        // Value is not accepted by slave
        IllegalDataValue = 3,
        // Unrecoverable error occurred while slave was attempting to perform requested action
        SlaveDeviceFailure = 4,
        // Slave has accepted request and is processing it, but a long duration of time is required.
        // This response is returned to prevent a timeout error from occurring in the master.
        // Master can next issue a Poll Program Complete message to determine whether processing is completed
        Acknowledge = 5,
        // Slave is engaged in processing a long-duration command. Master should retry later
        SlaveDeviceBusy = 6,
        // Slave cannot perform the programming functions. Master should request diagnostic or error information from slave
        NegativeAcknowledge = 7,
        // Slave detected a parity error in memory. Master can retry the request, but service may be required on the slave device
        MemoryParityError = 8,
        // Specialized for Modbus gateways. Indicates a misconfigured gateway
        GatewayPathUnavailable = 10,
        // Specialized for Modbus gateways. Sent when slave fails to respond
        GatewayTargetDeviceFailedToRespond = 11,

        CrcError = 64
    };
    Q_ENUM(Error)

    enum FunctionName {
        ReadDiscreteInputs = 2,
        ReadCoils = 1,
        WriteSingleCoil = 5,
        WriteMultipleCoils = 15,
        ReadInputRegisters = 4,
        ReadMultipleHoldingRegisters = 3,
        WriteSingleHoldingRegister = 6,
        WriteMultipleHoldingRegisters = 16,
        ReadOrWriteMultipleRegisters = 23,
        MaskWriteRegister = 22,
        ReadFIFOQueue = 24,
        ReadFileRecord = 20,
        WriteFileRecord = 21,
        ReadExceptionStatus = 7,
        Diagnostic = 8,
        GetComEventCounter = 11,
        GetComEventLog = 12,
        ReportSlaveID = 17,
        ReadDeviceIdentification = 43,
        EncapsulatedInterfaceTransport = 43,
    };
    Q_ENUM(FunctionName)

    // HoldingRegisters
    template <class T, class ByteOrdering = ByteOrder::NoReorder>
    bool readHoldingRegisters(uint16_t regAddress, T& regData, ByteOrdering ordering = {}) {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;

        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 125, "bad data size");

        QApplication::processEvents();
        QMutexLocker locker(&mutex);
        Timer<milliseconds> t(__FUNCTION__);
        bool ok{};

        prepare();

        { // write
            setStartOfRequest(m_address, ReadMultipleHoldingRegisters, regAddress);
            addToRequest16(size);
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }
        do { // read
            if (!readAndCheck())
                break;
            readResponse(response.packSize());
            logResponse();
            if (!checkCrc())
                break;

            Ty& tmp = *reinterpret_cast<T*>(response.pdu().adu1.data);
            ByteOrder::reorder(tmp, ordering);
            regData = tmp;
            ok = true;
        } while (0);

        return ok;
    }

    template <class T, class ByteOrdering = ByteOrder::NoReorder>
    bool writeHoldingRegisters(uint16_t regAddress, T&& regData, ByteOrdering ordering = {}) {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;
        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 123, "bad data size");

        QApplication::processEvents();
        QMutexLocker locker(&mutex);
        Timer t(__FUNCTION__);
        bool ok{};

        prepare();

        { // write
            auto tmp = regData;
            ByteOrder::reorder(tmp, ordering);
            if constexpr (size == 1) {
                setStartOfRequest(m_address, WriteSingleHoldingRegister, regAddress);
                addToRequestT(tmp);
            } else {
                setStartOfRequest(m_address, WriteMultipleHoldingRegisters, regAddress);
                addToRequest16(size);
                addToRequest08(size * 2);
                addToRequestT(tmp);
            }
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read

            if (!readAndCheck())
                break;
            readResponse(3);
            logResponse();
            if (!checkCrc())
                break;

            ok = true;
        } while (0);

        return ok;
    }

    // ReadInputRegisters
    template <class T, class ByteOrdering = ByteOrder::NoReorder>
    bool readInputRegisters(uint16_t regAddress, T& regData, ByteOrdering ordering = {}) {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;
        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 125, "bad data size");

        QApplication::processEvents();
        QMutexLocker locker(&mutex);
        Timer t(__FUNCTION__);
        bool ok{};

        prepare();
        { // write
            setStartOfRequest(m_address, ReadInputRegisters, regAddress);
            addToRequest16(size);
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read
            if (!readAndCheck())
                break;
            readResponse(response.packSize());
            logResponse();
            if (!checkCrc())
                break;
            Ty& tmp = *reinterpret_cast<T*>(response.pdu().adu1.data);
            ByteOrder::reorder(tmp, ordering);
            regData = tmp;
            ok = true;
        } while (0);

        return ok;
    }

    // Coils
    template <class T>
    bool readCoils(uint16_t regAddress, T& coils) {
        QApplication::processEvents();
        QMutexLocker locker(&mutex);
        Timer t(__FUNCTION__);
        bool ok{};

        prepare();

        { // write
            setStartOfRequest(m_address, ReadCoils, regAddress);
            if constexpr (/*std::size(coils) ||*/ std::is_array_v<T>) {
                addToRequest16(static_cast<uint16_t>(std::size(coils)));
            } else {
                addToRequest16(1);
            }
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read
            if (!readAndCheck())
                break;
            size_t size = *reinterpret_cast<uint8_t*>(response.data() + 2);
            readResponse(size);
            logResponse();
            if (!checkCrc())
                break;
            if constexpr (/*std::size(coils) || */ std::is_array_v<T>) {
                int ctr{};
                for (uint8_t c: std::span<uint8_t>{response.pdu().adu1.data, size}) {
                    uint8_t m{0x01};
                    for (int i = 1; i < 8; ++i, m <<= 1) {
                        coils[ctr++] = static_cast<bool>(c & m);
                        if (ctr == std::size(coils))
                            break;
                    }
                }
            } else {
                coils = static_cast<bool>(response[3]);
            }

            ok = true;
        } while (0);

        return ok;
    }

    template <class T>
    bool writeSingleCoil(uint16_t regAddress, T&& coils) {
        QApplication::processEvents();
        QMutexLocker locker(&mutex);

        Timer t(__FUNCTION__);
        bool ok{};

        prepare();
        { // write
            setStartOfRequest(m_address, WriteSingleCoil, regAddress);
            addToRequest16(static_cast<bool>(coils) ? 0xFF00 : 0x0000);
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read

            if (!readAndCheck())
                break;
            readResponse(3);
            logResponse();
            if (!checkCrc())
                break;

            ok = true;
        } while (0);

        return ok;
    }

    template <class T>
    bool writeMultipleCoils(uint16_t regAddress, T& coils)
        requires std::is_array_v<T>
    {
        constexpr size_t coilsCount = array_size(T{});
        constexpr uint8_t dataSize = coilsCount / 8 + (coilsCount % 8 ? 1 : 0);

        QApplication::processEvents();
        QMutexLocker locker(&mutex);
        Timer t(__FUNCTION__);
        bool ok{};

        prepare();
        { // write

            setStartOfRequest(m_address, WriteMultipleCoils, regAddress);
            addToRequest16(static_cast<uint16_t>(coilsCount));

            uint8_t data[dataSize]{};

            int ctr{};
            for (auto&& c: data) {
                for (int m = 1; m < 0x100; m <<= 1) {
                    if (coils[ctr++])
                        c |= m;
                    if (ctr == coilsCount)
                        break;
                }
            }

            addToRequest08(dataSize);

            addToRequestT(data);

            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read

            if (!readAndCheck())
                break;
            readResponse(3);
            logResponse();
            if (!checkCrc())
                break;

            ok = true;
        } while (0);

        return ok;
    }

    // DiscreteInputs
    template <class T>
    bool readDiscreteInputs(uint16_t regAddress, T& coils) {
        QApplication::processEvents();
        QMutexLocker locker(&mutex);

        Timer t(__FUNCTION__);
        bool ok{};

        prepare();
        { // write
            setStartOfRequest(m_address, ReadDiscreteInputs, regAddress);
            if constexpr (std::is_array_v<T>) {
                addToRequest16(static_cast<uint16_t>(std::size(coils)));
            } else {
                addToRequest16(1);
            }
            auto crc = crc16(request);
            addToRequest16(crc);
            writeRequest();
        }

        do { // read

            if (!readAndCheck())
                break;
            size_t size = *reinterpret_cast<uint8_t*>(response.data() + 2);
            readResponse(size);
            logResponse();
            if (!checkCrc())
                break;
            if constexpr (/*std::size(coils) || */ std::is_array_v<T>) {
                int ctr{};
                for (uint8_t c: std::span<uint8_t>{response.pdu().adu1.data, size}) {
                    uint8_t m{0x01};
                    for (int i = 1; i < 8; ++i, m <<= 1) {
                        coils[ctr++] = static_cast<bool>(c & m);
                        if (ctr == std::size(coils))
                            break;
                    }
                }
            } else {
                coils = static_cast<bool>(response[3]);
            }

            ok = true;
        } while (0);

        return ok;
    }

    uint8_t
    address() const;
    void setAddress(uint8_t newAddress);

    Error error() const;
    QString errorString() const { return m_errorString; }

    uint16_t crc16(std::span<uint8_t> data);

private:
    QMutex mutex;

    QSemaphore semaphore;

    QThread portThread;
    QSerialPort* m_port;

    int m_timeout{1000};

    uint8_t m_address{1};
    QString m_errorString;

    ByteVector request;
    ByteVector response;

    void prepare();

    void addToRequest08(uint8_t data) { request.emplace_back(uint8_t{data}); }

    void addToRequest16(uint16_t data) {
        U u{.u16 = data};
        request.emplace_back(u.u8[1]);
        request.emplace_back(u.u8[0]);
    }

    template <class T>
    void addToRequestT(T& data) {
        auto begin = reinterpret_cast<const uint8_t*>(std::addressof(data));
        auto end = begin + sizeof(T);
        request.insert(request.end(), begin, end);
    }

    bool readAndCheck();
    bool checkCrc();
    int readResponse(size_t count);

    void logRequest() { qDebug() << "request" << toHex(request); }
    void logResponse() { qDebug() << "response" << toHex(response); }

    int writeRequest();
    Error m_error{NoError};
    void setStartOfRequest(uint8_t address, FunctionName function, uint16_t regAddress) {
        addToRequest08(address);
        addToRequest08(function);
        addToRequest16(regAddress);
    }
};
