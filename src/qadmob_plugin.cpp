#include "qadmob_plugin.h"
#ifdef QADMOB_QT4
#include <QDeclarative.h>
#else
#include <QtQml/qqml.h>
#endif
#include "qadmob.h"
#include "adfonic/qadserviceplatformadfonic.h"

void QAdMobPlugin::registerTypes(const char *uri) {
    // @uri QtAdMob
    qmlRegisterType<QAdMob>(uri, 1, 0, "AdMob");
    qmlRegisterType<QAdMobAd>();
    qmlRegisterType<QAdServicePlatformAdfonic>(uri, 1, 0, "AdServicePlatformAdfonic");
}

#ifdef QADMOB_QT4
Q_EXPORT_PLUGIN2(QAdMobPlugin, QAdMobPlugin)
#endif
