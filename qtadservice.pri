lessThan(QT_MAJOR_VERSION, 5) {
DEFINES += QADSERVICE_QT4
}

INCLUDEPATH += \
    $$PWD/inc \
    $$PWD/platforms

SOURCES += \
    $$PWD/src/qadservice.cpp \
    $$PWD/src/qadservice_plugin.cpp \
    $$PWD/src/qad.cpp \
    $$PWD/src/qadserviceplatform.cpp \
    $$PWD/platforms/adfonic/qadserviceplatformadfonic.cpp

HEADERS += \
    $$PWD/inc/qadservice.h \
    $$PWD/inc/qadservice_plugin.h \
    $$PWD/inc/qad.h \
    $$PWD/inc/qadserviceplatform.h \
    $$PWD/platforms/adfonic/qadserviceplatformadfonic.h
