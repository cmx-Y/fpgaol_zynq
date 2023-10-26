TARGET = fpgaol_zynq
TEMPLATE = app
QT = core websockets
CONFIG += console

INCLUDEPATH += include/

HEADERS += include/log.h \
           include/fpga.h \
		   include/wsserver.h \
           include/httpserver.h

DEFINES += QT_MESSAGELOGCONTEXT \
#           QT_NO_DEBUG_OUTPUT

SOURCES += src/main.cpp \
           src/fpga.cpp \
		   src/wsserver.cpp \
           src/httpserver.cpp

include(./QtWebApp/httpserver/httpserver.pri)