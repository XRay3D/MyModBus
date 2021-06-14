#pragma once

#include <QApplication>
#include <QDebug>
#include <QSerialPort>
#include <array>

#define MODBUS_GET_HIGH_BYTE(data) (((data) >> 8) & 0xFF)
#define MODBUS_GET_LOW_BYTE(data) ((data)&0xFF)
#define MODBUS_GET_INT64_FROM_INT16(tab_int16, index) \
    (((int64_t)tab_int16[(index)] << 48) + ((int64_t)tab_int16[(index) + 1] << 32) + ((int64_t)tab_int16[(index) + 2] << 16) + (int64_t)tab_int16[(index) + 3])
#define MODBUS_GET_INT32_FROM_INT16(tab_int16, index) ((tab_int16[(index)] << 16) + tab_int16[(index) + 1])
#define MODBUS_GET_INT16_FROM_INT8(tab_int8, index) ((tab_int8[(index)] << 8) + tab_int8[(index) + 1])
#define MODBUS_SET_INT16_TO_INT8(tab_int8, index, value) \
    do {                                                 \
        tab_int8[(index)] = (value) >> 8;                \
        tab_int8[(index) + 1] = (value)&0xFF;            \
    } while(0)
#define MODBUS_SET_INT32_TO_INT16(tab_int16, index, value) \
    do {                                                   \
        tab_int16[(index)] = (value) >> 16;                \
        tab_int16[(index) + 1] = (value);                  \
    } while(0)
#define MODBUS_SET_INT64_TO_INT16(tab_int16, index, value) \
    do {                                                   \
        tab_int16[(index)] = (value) >> 48;                \
        tab_int16[(index) + 1] = (value) >> 32;            \
        tab_int16[(index) + 2] = (value) >> 16;            \
        tab_int16[(index) + 3] = (value);                  \
    } while(0)

/*


Таблица	╬Тип элемента	Тип доступа
Регистры флагов (Coils)	один бит	чтение и запись
Дискретные входы (Discrete Inputs)	один бит	только чтение
Регистры ввода (Input Registers)	16-битное слово	только чтение
Регистры хранения (Holding Registers)	16-битное слово	чтение и запись


PDU запроса и ответа для стандартных функций
номер
функции	запрос/ответ
1 (0x01)	A1	A0	Q1	Q0
                N	D (N байт)
2 (0x02)	A1	A0	Q1	Q0
                N	D (N байт)
3 (0x03)	A1	A0	Q1	Q0
                N	D (N байт)
4 (0x04)	A1	A0	Q1	Q0
                N	D (N байт)
5 (0x05)	A1	A0	D1	D0
                A1	A0	D1	D0
6 (0x06)	A1	A0	D1	D0
                A1	A0	D1	D0
15 (0x0F)	A1	A0	Q1	Q0	N	D (N байт)
                A1	A0	Q1	Q0
16 (0x10)	A1	A0	Q1	Q0	N	D (N байт)
                A1	A0	Q1	Q0

A1 и A0 — адрес элемента,
Q1 и Q0 — количество элементов,
N — количество байт данных
D — данные

Доступ к данным
Чтение данных
Для чтения значений из перечисленных выше таблиц данных используются функции с кодами 1—4 (шестнадцатеричные значения 0x01—0x04):
1 (0x01) — чтение значений из нескольких регистров флагов (Read Coil Status).
2 (0x02) — чтение значений из нескольких дискретных входов (Read Discrete Inputs).
3 (0x03) — чтение значений из нескольких регистров хранения (Read Holding Registers).
4 (0x04) — чтение значений из нескольких регистров ввода (Read Input Registers).
Запрос состоит из адреса первого элемента таблицы, значение которого требуется прочитать, и количества считываемых элементов. Адрес и количество данных задаются 16-битными числами, старший байт каждого из них передается первым.

В ответе передаются запрошенные данные. Количество байт данных зависит от количества запрошенных элементов. Перед данными передается один байт, значение которого равно количеству байт данных.

Значения регистров хранения и регистров ввода передаются начиная с указанного адреса, по два байта на регистр, старший байт каждого регистра передаётся первым:

байт 1	байт 2	байт 3	байт 4	…	байт N-1	байт N
RA,1	RA,0	RA+1,1	RA+1,0	…	RA+Q-1,1	RA+Q-1,0
Значения флагов и дискретных входов передаются в упакованном виде: по одному биту на флаг. Единица означает включённое состояние, ноль — выключенное. Значения запрошенных флагов заполняют сначала первый байт, начиная с младшего бита, затем следующие байты, также от младшего бита к старшим. Младший бит первого байта данных содержит значение флага, указанного в поле «адрес». Если запрошено количество флагов, не кратное восьми, то значения лишних битов заполняются нулями:

байт 1	…	байт N
FA+7	FA+6	FA+5	FA+4	FA+3	FA+2	FA+1	FA	…	0	…	0	FA+Q-1	FA+Q-2	…
Запись одного значения
5 (0x05) — запись значения одного флага (Force Single Coil).
6 (0x06) — запись значения в один регистр хранения (Preset Single Register).
Команда состоит из адреса элемента (2 байта) и устанавливаемого значения (2 байта).

Для регистра хранения значение является просто 16-битным словом.

Для флагов значение 0xFF00 означает включённое состояние, 0x0000 — выключенное, другие значения недопустимы.

Если команда выполнена успешно, ведомое устройство возвращает копию запроса.

Запись нескольких значений
15 (0x0F) — запись значений в несколько регистров флагов (Force Multiple Coils)
16 (0x10) — запись значений в несколько регистров хранения (Preset Multiple Registers)
Команда состоит из адреса элемента, количества изменяемых элементов, количества передаваемых байт устанавливаемых значений и самих устанавливаемых значений. Данные упаковываются так же, как в командах чтения данных.

Ответ состоит из начального адреса и количества изменённых элементов.

Изменение регистров
22 (0x16) — запись в один регистр хранения с использованием маски «И» и маски «ИЛИ» (Mask Write Register).
Команда состоит из адреса регистра и двух 16-битных чисел, которые используются как маски, с помощью которых можно индивидуально сбросить или установить отдельные биты в регистре. Конечный результат определяется формулой:

Результат = (Текущее_значение AND Маска_И) OR (Маска_ИЛИ AND (NOT Маска_И))

Чтение с записью
23 (0x17) — чтение/запись нескольких регистров (Read/Write Multiple registers)
Этот код функции выполняет комбинацию одной операции чтения и одной операции записи в одной транзакции MODBUS.

Очереди данных
24 (0x18) — чтение данных из очереди (Read FIFO Queue)
Функция предназначена для получения 16-битных слов из очереди, организованной по принципу «первым пришёл — первым ушёл» (FIFO).

Доступ к файлам
20 (0x14) — чтение из файла (Read File Record)
21 (0x15) — запись в файл (Write File Record)
Эти функции используются для доступа к 16-битным регистрам, организованным в файлы, состоящие из записей произвольной длины. В команде указывается номер файла, номер записи и длина записи в 16-битных словах. С помощью одной команды можно записать или прочитать несколько записей, не обязательно соседних.

Кроме того, команда содержит однобайтовый код для указания типа ссылки на данные. В действующей версии стандарта определен только один тип (описанный выше) с кодом 0x06.

Диагностика
Перечисленные ниже функции предназначены для устройств на последовательных линиях связи (Modbus RTU и Modbus ASCII).

7 (0x07) — чтение сигналов состояния (Read Exception Status)
Функция предназначена для получения информации об индикаторах состояния на удалённом устройстве. Функция возвращает один байт, каждый бит которого соответствует состоянию одного индикатора.

8 (0x08) — диагностика (Diagnostic)
11 (0x0B) — чтение счетчика событий (Get Com Event Counter)
12 (0x0C) — чтение журнала событий (Get Com Event Log)
Эти функции предназначены для проверки функционирования последовательной линии связи.

17 (0x11) — чтение информации об устройстве (Report Slave ID)
Функция предназначена для получения информации о типе устройства и его состоянии. Формат ответа зависит от устройства.

Другие
43 (0x2B) — Encapsulated Interface Transport
Функция предназначена для передачи данных в произвольных форматах (определённых другими стандартами) от ведущего (master) к ведомому (slave) и обратно.

Тип передаваемых данных определяется дополнительным кодом (MEI — MODBUS Encapsulated Interface), передаваемым после номера функции. Стандарт определяет MEI 13 (0x0D), предназначенный для инкапсуляции протокола CANopen. MEI 14 (0x0E) используется для получения информации об устройстве и MEI в диапазонах 0—12 и 15—255 зарезервированы.

Обработка ошибок
Во время обмена данными могут возникать ошибки двух типов:

ошибки, связанные с искажениями при передаче данных;
логические ошибки (запрос принят без искажений, но не может быть выполнен)
При передаче по асинхронным линиям связи ошибки первого типа обнаруживаются при помощи проверки соответствия принятого запроса установленному формату ADU и вычисления контрольной суммы. Дополнительно для проверки каждого символа может использоваться бит четности. Если подчинённое устройство обнаруживает искажение данных, принятый запрос игнорируется, ответное сообщение не формируется. Главное устройство может обнаружить ошибку по отсутствию ответа.

В Modbus TCP дополнительная проверка целостности данных не предусмотрена. Передача данных без искажений обеспечивается протоколами TCP/IP.

При ошибках второго типа подчинённое устройство отсылает сообщение об ошибке (если запрос адресован этому устройству; на широковещательные запросы ответ не отправляется). Признаком того, что ответ содержит сообщение об ошибке, является установленный старший бит номера функции. За номером функции, вместо обычных данных, следует код ошибки и, при необходимости, дополнительные данные об ошибке.

Стандартные коды ошибок
01 — Принятый код функции не может быть обработан.
02 — Адрес данных, указанный в запросе, недоступен.
03 — Значение, содержащееся в поле данных запроса, является недопустимой величиной.
04 — Невосстанавливаемая ошибка имела место, пока ведомое устройство пыталось выполнить затребованное действие.
05 — Ведомое устройство приняло запрос и обрабатывает его, но это требует много времени. Этот ответ предохраняет ведущее устройство от генерации ошибки тайм-аута.
06 — Ведомое устройство занято обработкой команды. Ведущее устройство должно повторить сообщение позже, когда ведомое освободится.
07 — Ведомое устройство не может выполнить программную функцию, заданную в запросе. Этот код возвращается для неуспешного программного запроса, использующего функции с номерами 13 или 14. Ведущее устройство должно запросить диагностическую информацию или информацию об ошибках от ведомого.
08 — Ведомое устройство при чтении расширенной памяти обнаружило ошибку контроля четности. Главный может повторить запрос позже, но обычно в таких случаях требуется ремонт оборудования.
Примеры
Ниже приведён пример команды ведущего устройства и ответов ведомого (для Modbus RTU).

Запрос
Направление передачи
Master→Slave
0x01    адрес подчинённого устройства
0x0F    номер функции
0x00    Адрес    старший байт
0x13    младший байт
0x00    Количество флагов   старший байт
0x0A    младший байт
0x02    Количество байт данных
0xCD    Данные  старший байт
0x01    младший байт
0x72    CRC старший байт
0xCB    младший байт
Ответ
Направление передачи
Slave→Master
0x01    адрес подчинённого устройства
0x0F    номер функции
0x00    Адрес	старший байт
0x13	младший байт
0x00    Количество флагов	старший байт
0x0A	младший байт
0x24    CRC младший байт
0x09    старший байт
Сообщение об ошибке
Направление передачи

Slave→Master
0x01	адрес подчинённого устройства
0x8F	номер функции
0x02	код ошибки
0xC5	CRC младший байт
0xF1	старший байт
https://ru.wikipedia.org/wiki/Modbus
https://habr.com/ru/company/advantech/blog/450234/



Code	Text	Details
1	Illegal Function	Function code received in the query is not recognized or allowed by slave
2	Illegal Data Address	Data address of some or all the required entities are not allowed or do not exist in slave
3	Illegal Data Value	Value is not accepted by slave
4	Slave Device Failure	Unrecoverable error occurred while slave was attempting to perform requested action
5	Acknowledge	Slave has accepted request and is processing it, but a long duration of time is required. This response is returned to prevent a timeout error from occurring in the master. Master can next issue a Poll Program Complete message to determine whether processing is completed
6	Slave Device Busy	Slave is engaged in processing a long-duration command. Master should retry later
7	Negative Acknowledge	Slave cannot perform the programming functions. Master should request diagnostic or error information from slave
8	Memory Parity Error	Slave detected a parity error in memory. Master can retry the request, but service may be required on the slave device
10	Gateway Path Unavailable	Specialized for Modbus gateways. Indicates a misconfigured gateway
11	Gateway Target Device Failed to Respond	Specialized for Modbus gateways. Sent when slave fails to respond


─━ │ ┃┄┅┆┇┈┉┊┋
┌┍┎┏
┐┑┒┓

└┕┖┗
┘┙┚┛
├┝┞┟┠┡┢┣

┤┥┦┧┨┩┪┫
┬┭┮┯┰┱┲┳

┴┵┶┷┸┹┺┻
┼┽┾┿╀╁╂╃╄╅╆╇╈╉╊╋╌╍╎╏═║
╒╓╔

╕╖╗

╘╙╚

╛╜╝

╞╟╠

╡╢╣

╤╥╦
╧

╨╩
╪╫╬╴╵╶╷╸╹╺╻╼╽╾╿╿

╱╲╳
╭╮
╯╰
┌┬─┐╔╤═╗╔╦═╗
├┼─┤╟┼─╢╠╬═╣
└┴─┘╚╧═╝╚╩═╝

Modbus function codes
╔═══════════════════════════════════════════════════════════════════════════════╦═══════════════════════════════════╦═══════════════╦═════════════╗
║ Function type                                                                 ║ Function name                     ║ Function code ║ Comment     ║
╠═════════════╤═══════════════╤═════════════════════════════════════════════════╩═══════════════════════════════════╩═══════════════╩═════════════╣
║             │               │ Physical Discrete Inputs                        │ Read Discrete Inputs              │ 2             │             ║*
║             │               ├─────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Read Coils                        │ 1             │             ║*
║             │ Bit access    │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║             │               │ Internal Bits or Physical Coils                 │ Write Single Coil                 │ 5             │             ║*
║             │               │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Write Multiple Coils              │ 15            │             ║*
║             ├───────────────┼─────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║             │               │ Physical Input Registers                        │ Read Input Registers              │ 4             │             ║*
║             │               ├─────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Read Multiple Holding Registers   │ 3             │             ║*
║             │               │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║ Data Access │               │                                                 │ Write Single Holding Register     │ 6             │             ║*
║             │               │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║             │ 16-bit access │                                                 │ Write Multiple Holding Registers  │ 16            │             ║*
║             │               │ Internal Registers or Physical Output Registers ├───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Read/Write Multiple Registers     │ 23            │             ║
║             │               │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Mask Write Register               │ 22            │             ║
║             │               │                                                 ├───────────────────────────────────┼───────────────┼─────────────╢
║             │               │                                                 │ Read FIFO Queue                   │ 24            │             ║
║             ├───────────────┴─────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║             │                                                                 │ Read File Record                  │ 20            │             ║
║             │ File Record Access                                              ├───────────────────────────────────┼───────────────┼─────────────╢
║             │                                                                 │ Write File Record                 │ 21            │             ║
╟─────────────┴─────────────────────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Read Exception Status             │ 7             │ serial only ║
║                                                                               ├───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Diagnostic                        │ 8             │ serial only ║
║                                                                               ├───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Get Com Event Counter             │ 11            │ serial only ║
║ Diagnostics                                                                   ├───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Get Com Event Log                 │ 12            │ serial only ║
║                                                                               ├───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Report Slave ID                   │ 17            │ serial only ║
║                                                                               ├───────────────────────────────────┼───────────────┼─────────────╢
║                                                                               │ Read Device Identification        │ 43            │             ║
╟───────────────────────────────────────────────────────────────────────────────┼───────────────────────────────────┼───────────────┼─────────────╢
║ Other                                                                         │ Encapsulated Interface Transport  │ 43            │             ║
╚═══════════════════════════════════════════════════════════════════════════════╧═══════════════════════════════════╧═══════════════╧═════════════╝

Object type         Access	Size	Address Space
Coil                Read-write	1 bit	00001 - 09999
Discrete input      Read-only	1 bit	10001 - 19999
Input register      Read-only	16 bits	30001 - 39999
Holding register    Read-write	16 bits	40001 - 49999

*/

namespace ByteOrder {
struct Swap16 {
    Swap16() { }
    template <size_t S>
    Swap16(std::array<uint16_t, S>& values) {
        for(auto& v : values) {
            union {
                uint8_t u8[2];
                uint16_t u16;
            } U{ .u16 = v };
            std::swap(U.u8[0], U.u8[1]);
        }
    }
    uint16_t operator()(uint16_t& val) {
        union {
            uint8_t u8[2];
            uint16_t u16;
        } U{ .u16 = val };
        std::swap(U.u8[0], U.u8[1]);
        return val;
    }
};

struct ReverseData {
    ReverseData() { }
    template <size_t S>
    ReverseData(std::array<uint16_t, S>& values) {
        std::ranges::reverse(values);
    }
};

using ABCD = std::tuple<>;
using BADC = std::tuple<Swap16>;
using CDAB = std::tuple<ReverseData>;
using DCBA = std::tuple<Swap16, ReverseData>;

}

struct RequestPdu
{
    RequestPdu() {}
};

class MyModbus : public QSerialPort {
    Q_OBJECT

    uint8_t m_address{ 1 };

public:
    explicit MyModbus(QObject* parent = nullptr);

    enum Error {
        //  Text  Code // Details
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
    } m_error;

    enum FunctionName {
        ReadDiscreteInputs = 2,
        ReadCoils = 1,
        WriteSingleCoil = 5,
        WriteMultipleCoils = 15,
        ReadInputRegisters = 4,
        ReadMultipleHoldingRegisters = 3, //
        WriteSingleHoldingRegister = 6,
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

    template <class T, class... ByteOrdering>
    bool readHoldingRegisters(uint16_t address, T& reg, std::tuple<ByteOrdering...>) {
        qDebug(__FUNCTION__);
        QApplication::processEvents();

        constexpr uint16_t size = sizeof(std::decay_t<T>) / 2;

        static_assert(!(sizeof(std::decay_t<T>) % 2), "bad data alinment");
        static_assert(size <= 125, "bad data size");

        QByteArray request;
        request.append(m_address);
        request.append(ReadMultipleHoldingRegisters);

        request.append((address >> 8) & 0xFF);
        request.append((address >> 0) & 0xFF);

        request.append((size >> 8) & 0xFF);
        request.append((size >> 0) & 0xFF);

        auto crc = crc16(request);

        request.append((crc >> 8) & 0xFF);
        request.append((crc >> 0) & 0xFF);

        qDebug() << "request" << request.toHex('|').toUpper();

        //flush();
        write(request);

        waitForReadyRead(100);

        QByteArray response{ readAll() };
        qDebug() << "response" << response.toHex('|').toUpper();

        using Array = std::array<uint16_t, size>;
        union U {
            Array data16{};
            T val;
        } u;
        u.data16 = *reinterpret_cast<Array*>(response.data() + 3);
        (ByteOrdering(u.data16), ...);
        reg = u.val;
        return true;
    }

    template <class T, class... ByteOrdering>
    bool writeHoldingRegisters(uint16_t address, T&& reg, std::tuple<ByteOrdering...>) {
        qDebug(__FUNCTION__);
        QApplication::processEvents();
        //        QByteArray data;
        //        data.append(m_address);

        //        uint16_t size = ceil(sizeof(T) / 2);
        //        union U {
        //            std::array<uint16_t, sizeof(T) / 2> data16{};
        //            std::decay_t<T> val;
        //        } u;

        //        u.val = reg;

        //        (ByteOrdering(u.data16), ...);

        //        if constexpr(sizeof(std::decay_t<T>) == 2) {
        //            data.append(m_address);
        //        } else {
        //            data.append(m_address);
        //        }
        return true;
    }

    //private:
    //    int set_slave(int slave);
    //    int get_slave();
    //    //    int set_error_recovery(error_recovery_mode error_recovery);
    //    int get_response_timeout(uint32_t* to_sec, uint32_t* to_usec);
    //    int set_response_timeout(uint32_t to_sec, uint32_t to_usec);
    //    int get_byte_timeout(uint32_t* to_sec, uint32_t* to_usec);
    //    int set_byte_timeout(uint32_t to_sec, uint32_t to_usec);
    //    int get_indication_timeout(uint32_t* to_sec, uint32_t* to_usec);
    //    int set_indication_timeout(uint32_t to_sec, uint32_t to_usec);
    //    int get_header_length();
    //    int connect();
    //    void close();
    //    void free();
    //    int flush();
    //    int set_debug(int flag);
    //    const char* strerror(int errnum);
    //    int read_registers(int addr, int nb, uint16_t* dest);
    //    int write_registers(int addr, int nb, const uint16_t* data);
    //    int read_bits(int addr, int nb, uint8_t* dest);
    //    int write_bits(int addr, int nb, const uint8_t* data);
    //    int write_bit(int coil_addr, int status);
    //    int read_input_bits(int addr, int nb, uint8_t* dest);
    //    int read_input_registers(int addr, int nb, uint16_t* dest);
    //    int write_register(int reg_addr, const uint16_t value);
    //    int mask_write_register(int addr, uint16_t and_mask, uint16_t or_mask);
    //    int write_and_read_registers(int write_addr, int write_nb, const uint16_t* src, int read_addr, int read_nb, uint16_t* dest);
    //    int report_slave_id(int max_dest, uint8_t* dest);
    //    //    mapping_t* mapping_new_start_address(unsigned int start_bits, unsigned int nb_bits, unsigned int start_input_bits, unsigned int nb_input_bits, unsigned int start_registers, unsigned int nb_registers, unsigned int start_input_registers, unsigned int nb_input_registers);
    //    //    mapping_t* mapping_new(int nb_bits, int nb_input_bits, int nb_registers, int nb_input_registers);
    //    //    void mapping_free(mapping_t* mb_mapping);
    //    int send_raw_request(const uint8_t* raw_req, int raw_req_length);
    //    int receive(uint8_t* req);
    //    int receive_confirmation(uint8_t* rsp);
    //    //    int reply(const uint8_t* req, int req_length, mapping_t* mb_mapping);
    //    int reply_exception(const uint8_t* req, unsigned int exception_code);
    //    // UTILS FUNCTIONS

    uint8_t address() const;
    void setAddress(uint8_t newAddress);

    Error error() const;
    QString errorString() const { return {}; }

    uint16_t crc16(const QByteArray& data);

signals:
};
