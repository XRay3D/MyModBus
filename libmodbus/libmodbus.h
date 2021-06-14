#ifndef LIBMODBUS_H
#define LIBMODBUS_H

#include <QApplication>
#include <QDebug>
#include <QObject>
#include <array>
#include <chrono>
#include <future>

struct _modbus;
typedef struct _modbus modbus_t;

class LibModbus : public QObject {
    Q_OBJECT

    Q_PROPERTY(DataBits dataBits READ dataBits WRITE setDataBits NOTIFY dataBitsChanged)
    Q_PROPERTY(Parity parity READ parity WRITE setParity NOTIFY parityChanged)
    Q_PROPERTY(StopBits stopBits READ stopBits WRITE setStopBits NOTIFY stopBitsChanged)
    Q_PROPERTY(qint32 baudRate READ baudRate WRITE setBaudRate NOTIFY baudRateChanged)

    modbus_t* mb = nullptr;
    bool m_connected{};

public:
    explicit LibModbus(QObject* parent = nullptr);
    ~LibModbus();

    enum BaudRate {
        Baud1200 = 1200,
        Baud2400 = 2400,
        Baud4800 = 4800,
        Baud9600 = 9600,
        Baud19200 = 19200,
        Baud38400 = 38400,
        Baud57600 = 57600,
        Baud115200 = 115200,
    };
    Q_ENUM(BaudRate)

    enum DataBits {
        Data5 = 5,
        Data6 = 6,
        Data7 = 7,
        Data8 = 8,
    };
    Q_ENUM(DataBits)

    enum Parity {
        NoParity = 'N',
        EvenParity = 'E',
        OddParity = 'O',

    };
    Q_ENUM(Parity)

    enum StopBits {
        OneStop = 1,
        TwoStop = 2,
    };
    Q_ENUM(StopBits)

    bool open();
    void close();

    QString errorString() const;

    void setPortName(const QString& portName);
    QString portName() const;

    bool setBaudRate(qint32 baudRate);
    qint32 baudRate() const;

    bool setDataBits(DataBits dataBits);
    DataBits dataBits() const;

    bool setParity(Parity parity);
    Parity parity() const;

    bool setStopBits(StopBits stopBits);
    StopBits stopBits() const;

    struct swap16 {
        swap16() { }
        template <size_t S>
        swap16(std::array<uint16_t, S>& values) {
            for(auto& v : values) {
                union {
                    uint8_t u8[2];
                    uint16_t u16;
                } U{.u16 = v};
                std::swap(U.u8[0], U.u8[1]);
            }
        }
    };

    struct reverseData {
        reverseData() { }
        template <size_t S>
        reverseData(std::array<uint16_t, S>& values) {
            std::ranges::reverse(values);
        }
    };

    using DataSR = std::tuple<swap16, reverseData>;
    using DataS = std::tuple<swap16>;
    using DataR = std::tuple<reverseData>;
    using DataN = std::tuple<>;

    template <class T, class... Mod>
    bool readRegisters(uint16_t regAddr, T& val, std::tuple<Mod...>) {
        qDebug(__FUNCTION__);
        using namespace std::chrono_literals;
        if(!m_connected && eventLoop.isRunning())
            return false;
        uint16_t size = ceil(sizeof(T) / 2);
        union U {
            std::array<uint16_t, sizeof(T) / 2> data16{};
            T val;
        } u;

        auto call = [&, this] { eventLoop.exit(modbusReadRegisters(regAddr, size, u.data16.data())); };
        //std::future<int> future = std::async(std::launch::async, modbus_read_registers, mb, regAddr, size, data.data());
        std::future future = std::async(std::launch::async, call);
        if(int rc = eventLoop.exec(); rc != size)
            return false;

        qDebug() << QByteArray(reinterpret_cast<const char*>(u.data16.data()), u.data16.size() * 2).toHex('|').toUpper();

        (Mod(u.data16), ...);

        val = u.val;
        return true;
    }

    template <class T, class... Mod>
    bool writeRegisters(uint16_t regAddr, const T& val, std::tuple<Mod...>) {
        qDebug(__FUNCTION__);
        using namespace std::chrono_literals;
        if(!m_connected && eventLoop.isRunning())
            return false;
        uint16_t size = ceil(sizeof(T) / 2);
        union U {
            std::array<uint16_t, sizeof(T) / 2> data16{};
            T val;
        } u;

        u.val = val;

        (Mod(u.data16), ...);

        qDebug() << QByteArray(reinterpret_cast<const char*>(u.data16.data()), u.data16.size() * 2).toHex('|').toUpper();

        auto call = [&, this] { eventLoop.exit(modbusWriteRegisters(regAddr, size, u.data16.data())); };
        std::future future = std::async(std::launch::async, call);
        if(int rc = eventLoop.exec(); rc != size)
            return false;
        return true;
    }

    int modbusReadRegisters(int addr, int nb, uint16_t* dest) const;
    int modbusWriteRegisters(int addr, int nb, uint16_t* dest) const;

signals:
    void baudRateChanged(qint32 baudRate);
    void dataBitsChanged(LibModbus::DataBits dataBits);
    void parityChanged(LibModbus::Parity parity);
    void stopBitsChanged(LibModbus::StopBits stopBits);

private:
    QEventLoop eventLoop;

    DataBits m_dataBits{Data8};
    Parity m_parity{NoParity};
    QString m_portName{};
    StopBits m_stopBits{OneStop};
    qint32 m_baudRate{Baud115200};
};

#endif // LIBMODBUS_H
