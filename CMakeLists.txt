cmake_minimum_required(VERSION 3.5)

project(ModBusTester VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets SerialPort)

add_subdirectory(xrmodbus)

set(PROJECT_SOURCES
    DataBase.cpp
    EnumHelper.h
    coils/coils_model.cpp
    coils/coils_model.h
    discreteinputs/di_model.cpp
    discreteinputs/di_model.h
    holdingregisters/hldreg_data.cpp
    holdingregisters/hldreg_data.h
    holdingregisters/hldreg_delegate.cpp
    holdingregisters/hldreg_delegate.h
    holdingregisters/hldreg_model.cpp
    holdingregisters/hldreg_model.h
    inputregisters/inreg_data.cpp
    inputregisters/inreg_data.h
    inputregisters/inreg_delegate.cpp
    inputregisters/inreg_delegate.h
    inputregisters/inreg_model.cpp
    inputregisters/inreg_model.h
    interface.cpp
    interface.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    modelinputdiscrete.cpp
    modelinputdiscrete.h
    modelinputregisters.cpp
    modelinputregisters.h
    timer.h

    mainwindow.ui

    # qml/qml.qrc
    Image.qrc
)
# file(GLOB PROJECT_SOURCES
#     *.cpp
#     *.h
#     *.ui
#     *.qrc
#     *.json
# )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
    # Define properties for Android with Qt 5 after find_package() calls as:
    #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    XrModBus
    Qt${QT_VERSION_MAJOR}::SerialPort
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()


if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE setupapi)
    # windeployqt
    set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
    get_target_property(QMAKE_EXECUTABLE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${QT_BIN_DIR}" "${WINDEPLOYQT_EXECUTABLE}"
        --verbose 1
        --no-compiler-runtime
        \"$<TARGET_FILE:${PROJECT_NAME}>\"
        COMMENT "Deploying Qt..."
    )
elseif (UNIX____)#LINUX

    find_program(Q_MAKE_EXE qmake)

    find_program(CQT_EXE cqtdeployer)

    if(NOT EXISTS ${CQT_EXE})
        message("the cqtdeployer not exits please install the cqtdeployer and run cmake again!")
        return()
    endif()

    add_custom_target(
        deploy
        COMMAND cqtdeployer clear -bin ${CURENT_PROJECT} -qmake ${Q_MAKE_EXE} -libDir ${PROJECT_SOURCE_DIR} -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 5
        COMMENT "Deploy: cqtdeployer clear -bin ${CURENT_PROJECT} -qmake ${Q_MAKE_EXE} -libDir ${PROJECT_SOURCE_DIR} -qmlDir ${PROJECT_SOURCE_DIR} -recursiveDepth 5"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )


# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
# COMMAND "${CMAKE_COMMAND}" -E
# env PATH="${_QT_BIN_DIR}" "cqtdeployer"
# -bin $<TARGET_FILE:${PROJECT_NAME}>
# -qmake ${QT_QMAKE_EXECUTABLE}
# qif
# COMMENT "Deploying Qt..."
# )
# cqtdeployer -bin myApp -qmake path/to/Qt/5.x.x/build/bin/qmake -qmlDir path/to/my/qml/files/dir qif

endif()


