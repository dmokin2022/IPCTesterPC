QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ConfigInterpretator.cpp \
    TerminalFrame.cpp \
    ParameterView.cpp \
    Command/CameraCommand.cpp \
    Command/StandCommand.cpp \
    Connector/Connector.cpp \
    Connector/IpValidator.cpp \
    MainWindow.cpp \
    TestData.cpp \
    Tester.cpp \
    Variable.cpp \
    main.cpp


HEADERS += \
    ConfigInterpretator.hpp \
    TerminalFrame.hpp \
    ParameterView.hpp \
    Command/CameraCommand.hpp \
    Command/StandCommand.hpp \
    Connector/Connector.hpp \
    Connector/IpValidator.hpp \
    Configs/StandConfig.h \
    Configs/CameraConfig.h \
    Configs/Thresholds.h \
    Configs/TestNames.h \
    Configs/Config.h \
    MainWindow.hpp \
    TestData.hpp \
    Tester.hpp \
    Variable.hpp

FORMS += \
    MainWindow.ui \
    TerminalFrame.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT += network
QT += concurrent
QT += multimediawidgets
