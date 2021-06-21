QT += core gui serialport serialbus #quick

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += debug_and_release

TARGET = "FlowMeter"

#CONFIG += c++17
CONFIG(debug, debug|release) {
    TARGET = $$TARGET"_debug"
} else {
    TARGET = $$TARGET"_release"
}

DESTDIR = $$_PRO_FILE_PWD_/bin/

QMAKE_CXXFLAGS += /std:c++latest
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    DataBase.cpp \
    coils/coils_model.cpp \
    holdingregisters/hldreg_data.cpp \
    holdingregisters/hldreg_delegate.cpp \
    holdingregisters/hldreg_model.cpp \
    interface.cpp \
    main.cpp \
    mainwindow.cpp \
    modelinputdiscrete.cpp \
    modelinputregisters.cpp \


HEADERS += \
    EnumHelper.h \
    coils/coils_model.h \
    holdingregisters/hldreg_data.h \
    holdingregisters/hldreg_delegate.h \
    holdingregisters/hldreg_model.h \
    interface.h \
    mainwindow.h \
    modelinputdiscrete.h \
    modelinputregisters.h \

FORMS += \
    mainwindow.ui


TRANSLATIONS += \
    FMeter_Clb_ru_RU.ts

DISTFILES += \
#    AddrId.c \
#    DataBase.c \

INCLUDEPATH += \
    magic_get/include/ \
    holdingregisters \

include(libmodbus/libmodbus.pri)

# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}/bin
#!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Image.qrc \
    qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

