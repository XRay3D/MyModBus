#include "interface.h"

I::I() {
    if (!semaphore.available()) {
        //        m_modbus = new LibModbus();
        //        m_modbus->moveToThread(&thread);
        //        thread.connect(&thread, &QThread::finished, m_modbus, &QObject::deleteLater);

        m_mymodbus = new Modbus();
        /// m_mymodbus->moveToThread(&thread);
        /// thread.connect(&thread, &QThread::finished, m_mymodbus, &QObject::deleteLater);

        //        thread.start(QThread::NormalPriority);
    }
    semaphore.release();
}

I::~I() {
    semaphore.acquire();
    if (!semaphore.available()) {
        /*        thread.quit();
        thread.wait()*/
        ;
    }
}

// LibModbus* I::modbus() { return m_modbus; }
Modbus* I::mymodbus() { return m_mymodbus; }
