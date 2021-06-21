#pragma once

#include "EnumHelper.h"
#include "modbusutility.h"
#include <QApplication>
#include <QDebug>
#include <QSerialPort>

#pragma pack(push, 1)

struct PDU {
    uint8_t address;
    struct ADU {
        uint8_t functionCode;
        uint8_t data[1];
    } adu;
    uint16_t crc() { return {}; };
};

#pragma pack(pop)

class MyModbus : public QSerialPort {
    Q_OBJECT

    uint8_t m_address { 1 };
    QString m_errorString;

    QByteArray request;
    QByteArray response;

    void prepare()
    {
        request.clear();
        response.clear();
        m_errorString.clear();
        readAll();
        QApplication::processEvents();
    }

public:
    explicit MyModbus(QObject* parent = nullptr);

    enum Error {
        //  Text  Code // Details
        NoError = 0,
        IllegalFunction = 1, //Function code received in the query is not recognized or allowed by slave
        IllegalDataAddress = 2, //Data address of some or all the required entities are not allowed or do not exist in slave
        IllegalDataValue = 3, //Value is not accepted by slave
        SlaveDeviceFailure = 4, //Unrecoverable error occurred while slave was attempting to perform requested action
        Acknowledge = 5, //Slave has accepted request and is processing it, but a long duration of time is required. This response is returned to prevent a timeout error from occurring in the master. Master can next issue a Poll Program Complete message to determine whether processing is completed
        SlaveDeviceBusy = 6, //Slave is engaged in processing a long-duration command. Master should retry later
        NegativeAcknowledge = 7, //Slave cannot perform the programming functions. Master should request diagnostic or error information from slave
        MemoryParityError = 8, //Slave detected a parity error in memory. Master can retry the request, but service may be required on the slave device
        GatewayPathUnavailable = 10, //Specialized for Modbus gateways. Indicates a misconfigured gateway
        GatewayTargetDeviceFailedToRespond = 11, //Specialized for Modbus gateways. Sent when slave fails to respond
    } m_error {};
    Q_ENUM(Error)
    enum FunctionName {
        ReadDiscreteInputs = 2,
        ReadCoils = 1,
        WriteSingleCoil = 5,
        WriteMultipleCoils = 15,
        ReadInputRegisters = 4,
        ReadMultipleHoldingRegisters = 3, //
        WriteSingleHoldingRegister = 6, //
        WriteMultipleHoldingRegisters = 16, //
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

    void add08(QByteArray& arr, uint8_t data) { arr.append(data); }
    void add16(QByteArray& arr, uint16_t data)
    {
        union {
            uint16_t d;
            uint8_t byte[2];
        } u { .d = data };
        arr.append(u.byte[1]);
        arr.append(u.byte[0]);
    }
    template <class T>
    void addT(QByteArray& arr, T& data) { arr.append(QByteArray::fromRawData(reinterpret_cast<const char*>(std::addressof(data)), sizeof(T))); }
    ///////////////////////////////////// HoldingRegisters
    template <class T, class... ByteOrdering>
    bool readHoldingRegisters(uint16_t regAddress, T& regData, std::tuple<ByteOrdering...>)
    {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;
        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 125, "bad data size");

        qDebug(__FUNCTION__);
        prepare();
        { // write
            add08(request, m_address);
            add08(request, ReadMultipleHoldingRegisters);
            add16(request, regAddress);
            add16(request, size);
            auto crc = crc16(request);
            add16(request, crc);
            qDebug() << "request" << request.toHex('|').toUpper();
            write(request);
        }

        { // read
            waitForReadyRead(100);
            response.append(read(5));
            if (response[1] & 0x80) {
                m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
                qDebug() << "err response" << response.toHex('|').toUpper();
                return {};
            }
            response.append(read(*reinterpret_cast<uint16_t*>(response.data() + 2) + 1));
            qDebug() << "response" << response.toHex('|').toUpper();
            Ty& tmp = *reinterpret_cast<T*>(response.data() + 3);
            ByteOrder::reorder<Ty, ByteOrdering...>(tmp);
            regData = tmp;
        }
        return true;
    }

    template <class T, class... ByteOrdering>
    bool writeHoldingRegisters(uint16_t regAddress, T&& regData, std::tuple<ByteOrdering...>)
    {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;
        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 123, "bad data size");

        qDebug(__FUNCTION__);
        prepare();
        { // write
            auto tmp = regData;
            ByteOrder::reorder<Ty, ByteOrdering...>(tmp);
            add08(request, m_address);
            if constexpr (size == 1) {
                add08(request, WriteSingleHoldingRegister);
                add16(request, regAddress);
                addT(request, tmp);
            } else {
                add08(request, WriteMultipleHoldingRegisters);
                add16(request, regAddress);
                add16(request, size);
                add08(request, size * 2);
                addT(request, tmp);
            }
            auto crc = crc16(request);
            add16(request, crc);
            qDebug() << "request" << request.toHex('|').toUpper();
            write(request);
        }

        { // read
            waitForReadyRead(1000);
            response.append(read(5));
            if (response[1] & 0x80) {
                m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
                qDebug() << "err response" << response.toHex('|').toUpper();
                return {};
            }
            response.append(read(3));
            qDebug() << "response" << response.toHex('|').toUpper();
        }
        return true;
    }

    ///////////////////////////////////// Coils
    template <class T>
    bool readCoils(uint16_t regAddress, T& coils)
    {
        qDebug(__FUNCTION__);
        prepare();
        {
            add08(request, m_address);
            add08(request, ReadCoils);
            add16(request, regAddress);
            if constexpr (/*std::size(coils) ||*/ std::is_array_v<T>) {
                add16(request, static_cast<uint16_t>(std::size(coils)));
            } else {
                add16(request, 1);
            }
            auto crc = crc16(request);
            add16(request, crc);
            qDebug() << "request" << request.toHex('|').toUpper();
            write(request);
        }

        { // read
            waitForReadyRead(1000);
            response.append(read(5));
            if (response[1] & 0x80) {
                m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
                qDebug() << "err response" << response.toHex('|').toUpper();
                return {};
            }
            int size = *reinterpret_cast<uint8_t*>(response.data() + 2);
            response.append(read(size));
            if constexpr (/*std::size(coils) || */ std::is_array_v<T>) {
                int ctr {};
                for (char c : response.mid(3, size)) {
                    for (int m = 1; m < 0x100; m <<= 1) {
                        coils[ctr++] = static_cast<bool>(c & m);
                        if (ctr == std::size(coils))
                            break;
                    }
                }
            } else {
                coils = static_cast<bool>(response[3]);
            }
            qDebug() << "response" << response.toHex('|').toUpper();
        }
        return true;
    }
    template <class T>
    bool writeSingleCoil(uint16_t regAddress, T&& coils)
    {
        qDebug(__FUNCTION__);
        prepare();
        { // write
            add08(request, m_address);
            add08(request, WriteSingleCoil);
            add16(request, regAddress);
            add16(request, static_cast<bool>(coils) ? 0xFF00 : 0x0000);
            auto crc = crc16(request);
            add16(request, crc);
            qDebug() << "request" << request.toHex('|').toUpper();
            write(request);
        }

        { // read
            waitForReadyRead(1000);
            response.append(read(5));
            if (response[1] & 0x80) {
                m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
                qDebug() << "err response" << response.toHex('|').toUpper();
                return {};
            }
            response.append(read(3));
            qDebug() << "response" << response.toHex('|').toUpper();
        }
        return true;
    }

    template <typename T, size_t N>
    struct array_size {
        constexpr array_size(T (&)[N]) { }
        constexpr array_size(T(&&)[N]) { }
        constexpr operator size_t() { return N; }
    };
    template <typename T, size_t N>
    array_size(T (&)[N]) -> array_size<T, N>;
    template <typename T, size_t N>
    array_size(T(&&)[N]) -> array_size<T, N>;

    void check(int const (&param)[3])
    {
        int local[] { 1, 2, 3 };
        [[maybe_unused]] constexpr size_t s0 = array_size(local); // ok
        [[maybe_unused]] constexpr size_t s1 = array_size(decltype(param) {}); // ok
    }

    template <class T>
    bool writeMultipleCoils(uint16_t regAddress, T& coils) requires std::is_array_v<T>
    {
        qDebug(__FUNCTION__);
        prepare();
        { // write
            constexpr size_t coilsCount = array_size(T {});
            constexpr uint8_t dataSize = coilsCount / 8 + (coilsCount % 8 ? 1 : 0);

            add08(request, m_address);
            add08(request, WriteMultipleCoils);
            add16(request, regAddress);
            add16(request, static_cast<uint16_t>(coilsCount));

            uint8_t data[dataSize] {};

            int ctr {};
            for (auto&& c : data) {
                for (int m = 1; m < 0x100; m <<= 1) {
                    if (coils[ctr++])
                        c |= m;
                    if (ctr == coilsCount)
                        break;
                }
            }

            add08(request, dataSize);

            addT(request, data);

            auto crc = crc16(request);
            add16(request, crc);
            qDebug() << "request" << request.toHex('|').toUpper();
            write(request);
        }

        { // read
            waitForReadyRead(1000);
            response.append(read(5));
            if (response[1] & 0x80) {
                m_errorString = EnumHelper::toString(m_error = static_cast<Error>(response.data()[2]));
                qDebug() << "err response" << response.toHex('|').toUpper();
                return {};
            }
            response.append(read(3));
            qDebug() << "response" << response.toHex('|').toUpper();
        }
        return true;
    }

    uint8_t address() const;
    void setAddress(uint8_t newAddress);

    Error error() const;
    QString errorString() const { return m_errorString; }

    uint16_t crc16(const QByteArray& data);

signals:
};
