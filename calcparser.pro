# (c) 2018 Jens Kallup !
QT       += gui widgets

TARGET = calcparser
TEMPLATE = lib

DEFINES += CALCPARSER_LIBRARY
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        calcparser.cc \
        p1.cc \
        p1.tab.cc

HEADERS += \
        calcparser.h \
        calcparser_global.h

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}

DISTFILES += \
    source.calc \
    p1.lpp \
    p1.ypp

LIBS += -ly
