#include "qadservice_plugin.h"
#ifdef QADSERVICE_QT4
#include <QDeclarative.h>
#else
#include <QtQml/qqml.h>
#endif
#include "qadservice.h"
#include "adfonic/qadplatformadfonic.h"
#include "inneractive/qadplatforminneractive.h"
#include "mojiva/qadplatformmojiva.h"

void QAdServicePlugin::registerTypes(const char *uri) {
    Q_ASSERT(QLatin1String(uri) == QLatin1String("QtAdService"));
    Q_UNUSED(uri);
    qmlRegisterType<QAdService>(uri, 1, 0, "AdService");
    qmlRegisterType<QAd>();
    qmlRegisterType<QAdPlatformAdfonic>(uri, 1, 0, "AdPlatformAdfonic");
    qmlRegisterType<QAdPlatformInneractive>(uri, 1, 0, "AdPlatformInneractive");
    qmlRegisterType<QAdPlatformMojiva>(uri, 1, 0, "AdPlatformMojiva");
}

#ifdef QADSERVICE_QT4
Q_EXPORT_PLUGIN2(QAdServicePlugin, QAdServicePlugin)
#endif
