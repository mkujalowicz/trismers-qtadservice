lessThan(QT_MAJOR_VERSION, 5) {
DEFINES += QADSERVICE_QT4
}

QT += xml

INCLUDEPATH += \
    $$PWD/inc \
    $$PWD/platforms

SOURCES += \
    $$PWD/src/qadservice.cpp \
    $$PWD/src/qadservice_plugin.cpp \
    $$PWD/src/qad.cpp \
    $$PWD/src/qadplatform.cpp \
    $$PWD/platforms/adfonic/qadplatformadfonic.cpp \
    $$PWD/platforms/inneractive/qadplatforminneractive.cpp

HEADERS += \
    $$PWD/inc/qadservice.h \
    $$PWD/inc/qadservice_plugin.h \
    $$PWD/inc/qad.h \
    $$PWD/inc/qadplatform.h \
    $$PWD/platforms/adfonic/qadplatformadfonic.h \
    $$PWD/platforms/inneractive/qadplatforminneractive.h
