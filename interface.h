#pragma once

#include "libmodbus.h"
#include <QSemaphore>
#include <QThread>

class I {
    inline static LibModbus* m_modbus = nullptr;
    inline static QSemaphore semaphore;
    inline static QThread thread;

public:
    I();
    ~I();

    static LibModbus* modbus();
};
