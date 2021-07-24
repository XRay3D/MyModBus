#pragma once

//#include "libmodbus.h"
#include <QSemaphore>
#include <QThread>
#include <mymodbus.h>

class I {
//    inline static LibModbus* m_modbus = nullptr;
    inline static QSemaphore semaphore;
    inline static QThread thread;
    inline static Modbus* m_mymodbus = nullptr;

public:
    I();
    ~I();

//    static LibModbus* modbus();
    static Modbus* mymodbus();
};
