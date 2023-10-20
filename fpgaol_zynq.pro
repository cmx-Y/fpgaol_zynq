TARGET = fpgaol_zynq
TEMPLATE = app
QT = core
CONFIG += console

INCLUDEPATH += include/

HEADERS += include/log.h \
           include/fpga.h

DEFINES += QT_MESSAGELOGCONTEXT

SOURCES += src/main.cpp

