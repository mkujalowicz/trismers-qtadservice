lessThan(QT_MAJOR_VERSION, 5) {
DEFINES += QADMOB_QT4
}

INCLUDEPATH += \
    $$PWD/inc \
    $$PWD/platforms

SOURCES += \
    $$PWD/src/qadmob.cpp \
    $$PWD/src/qadmob_plugin.cpp \
    $$PWD/src/qadmobad.cpp \
    $$PWD/src/qadserviceplatform.cpp \
    $$PWD/platforms/adfonic/qadserviceplatformadfonic.cpp

HEADERS += \
    $$PWD/inc/qadmob.h \
    $$PWD/inc/qadmob_plugin.h \
    $$PWD/inc/qadmobad.h \
    $$PWD/inc/qadserviceplatform.h \
    $$PWD/platforms/adfonic/qadserviceplatformadfonic.h

