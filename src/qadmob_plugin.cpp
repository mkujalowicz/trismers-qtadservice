#include "qadmob_plugin.h"
#ifdef QADMOB_QT4
#include <QDeclarative.h>
#else
#include <QtQml/qqml.h>
#endif
#include "qadmob.h"

void QAdMobPlugin::registerTypes(const char *uri) {
    // @uri com.trismer.components
    qmlRegisterType<QAdMob>(uri, 1, 0, "AdMob");
}

#ifdef QADMOB_QT4
Q_EXPORT_PLUGIN2(QAdMobPlugin, QAdMobPlugin)
#endif
