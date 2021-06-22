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

using ByteVector = std::vector<std::byte>;

class MyModbus : public QSerialPort {
    Q_OBJECT

    uint8_t m_address { 1 };
    QString m_errorString;

    ByteVector request;
    ByteVector response;

    void prepare();

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
            writeRequest();
        }

        { // read
            waitForReadyRead(100);
            if (!readAndCheck())
                return false;
            readResponse(*reinterpret_cast<uint16_t*>(response.data() + 2) + 1);
            logResponse();
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
            writeRequest();
        }

        { // read
            waitForReadyRead(1000);
            if (!readAndCheck())
                return false;
            readResponse(3);
            logResponse();
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
            writeRequest();
        }

        { // read
            waitForReadyRead(1000);
            if (!readAndCheck())
                return false;
            size_t size = *reinterpret_cast<uint8_t*>(response.data() + 2);
            readResponse(size);
            if constexpr (/*std::size(coils) || */ std::is_array_v<T>) {
                int ctr {};
                for (std::byte c : std::span<std::byte> { response.begin() + 3, size }) {
                    std::byte m { 0x01 };
                    for (int i = 1; i < 8; ++i, m <<= 1) {
                        coils[ctr++] = static_cast<bool>(c & m);
                        if (ctr == std::size(coils))
                            break;
                    }
                }
            } else {
                coils = static_cast<bool>(response[3]);
            }
            logResponse();
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
            writeRequest();
        }

        { // read
            waitForReadyRead(1000);
            if (!readAndCheck())
                return false;
            readResponse(3);
            logResponse();
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
            writeRequest();
        }

        { // read
            waitForReadyRead(1000);
            if (!readAndCheck())
                return false;
            readResponse(3);
            logResponse();
        }
        return true;
    }

    ///////////////////////////////////// DiscreteInputs
    template <class T>
    bool readDiscreteInputs(uint16_t regAddress, T& coils)
    {
        qDebug(__FUNCTION__);
        prepare();
        {
            add08(request, m_address);
            add08(request, ReadDiscreteInputs);
            add16(request, regAddress);
            if constexpr (/*std::size(coils) ||*/ std::is_array_v<T>) {
                add16(request, static_cast<uint16_t>(std::size(coils)));
            } else {
                add16(request, 1);
            }
            auto crc = crc16(request);
            add16(request, crc);
            writeRequest();
        }

        { // read
            waitForReadyRead(1000);
            if (!readAndCheck())
                return false;
            size_t size = *reinterpret_cast<uint8_t*>(response.data() + 2);
            readResponse(size);
            if constexpr (/*std::size(coils) || */ std::is_array_v<T>) {
                int ctr {};
                for (std::byte c : std::span<std::byte> { response.begin() + 3, size }) {
                    std::byte m { 0x01 };
                    for (int i = 1; i < 8; ++i, m <<= 1) {
                        coils[ctr++] = static_cast<bool>(c & m);
                        if (ctr == std::size(coils))
                            break;
                    }
                }
            } else {
                coils = static_cast<bool>(response[3]);
            }
            logResponse();
        }
        return true;
    }

    ///////////////////////////////////// ReadInputRegisters
    template <class T, class... ByteOrdering>
    bool readInputRegisters(uint16_t regAddress, T& regData, std::tuple<ByteOrdering...>)
    {
        using Ty = std::decay_t<T>;
        constexpr uint16_t size = sizeof(Ty) / 2;
        static_assert(!(sizeof(Ty) % 2), "bad data alinment");
        static_assert(size <= 125, "bad data size");

        qDebug(__FUNCTION__);
        prepare();
        { // write
            add08(request, m_address);
            add08(request, ReadInputRegisters);
            add16(request, regAddress);
            add16(request, size);
            auto crc = crc16(request);
            add16(request, crc);
            writeRequest();
        }

        { // read
            waitForReadyRead(100);
            if (!readAndCheck())
                return false;
            readResponse(*reinterpret_cast<uint16_t*>(response.data() + 2) + 1);
            logResponse();
            Ty& tmp = *reinterpret_cast<T*>(response.data() + 3);
            ByteOrder::reorder<Ty, ByteOrdering...>(tmp);
            regData = tmp;
        }
        return true;
    }

    uint8_t address() const;
    void setAddress(uint8_t newAddress);

    Error error() const;
    QString errorString() const { return m_errorString; }

    uint16_t crc16(std::span<std::byte> data);

private:
    void add08(ByteVector& arr, uint8_t data);
    void add16(ByteVector& arr, uint16_t data);
    template <class T>
    void addT(ByteVector& arr, T& data)
    {
        auto begin = reinterpret_cast<const std::byte*>(std::addressof(data));
        auto end = reinterpret_cast<const std::byte*>(std::addressof(data)) + sizeof(T);
        arr.insert(arr.end(), begin, end);
    }

    bool readAndCheck();
    qint64 readResponse(int count);

    void logResponse();
    void logRequest();
    qint64 writeRequest();

signals:
};
