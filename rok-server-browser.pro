TARGET = rok-server-browser
QT += widgets network
QMAKE_LFLAGS += -static -static-libgcc
CONFIG += static release
INCLUDEPATH += ./include
VPATH += ./src ./include
HEADERS += server-browser.hpp
SOURCES += main.cpp server-browser.cpp