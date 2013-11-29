lessThan(QT_MAJOR_VERSION, 5) {
DEFINES += QADMOB_QT4
}

INCLUDEPATH += \
    $$PWD/inc

SOURCES += \
    $$PWD/src/qadmob.cpp \
    $$PWD/src/qadmob_plugin.cpp

HEADERS += \
    $$PWD/inc/qadmob.h \
    $$PWD/inc/qadmob_plugin.h

