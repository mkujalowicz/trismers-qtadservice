#ifndef QADMOB_PLUGIN_H
#define QADMOB_PLUGIN_H

#include <QtGlobal>

#ifdef QADMOB_QT4
#include <QtDeclarative/QDeclarativeExtensionPlugin>
#else
#include <QtQml/QQmlExtensionPlugin>
#endif

class QAdMobPlugin: public
#ifdef QADMOB_QT4
       QDeclarativeExtensionPlugin
#else
       QQmlExtensionPlugin
#endif
{
    Q_OBJECT
#ifndef QADMOB_QT4
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "plugin.json")
#endif
public:
    void registerTypes(const char *uri);
};

#endif // QADMOB_PLUGIN_H

