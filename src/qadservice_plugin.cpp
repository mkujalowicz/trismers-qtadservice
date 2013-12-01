#include "qadservice_plugin.h"
#ifdef QADSERVICE_QT4
#include <QDeclarative.h>
#else
#include <QtQml/qqml.h>
#endif
#include "qadservice.h"
#include "adfonic/qadplatformadfonic.h"

void QAdServicePlugin::registerTypes(const char *uri) {
    // @uri QtAdService
    qmlRegisterType<QAdService>(uri, 1, 0, "AdService");
    qmlRegisterType<QAd>();
    qmlRegisterType<QAdPlatformAdfonic>(uri, 1, 0, "AdPlatformAdfonic");
}

#ifdef QADSERVICE_QT4
Q_EXPORT_PLUGIN2(QAdServicePlugin, QAdServicePlugin)
#endif
