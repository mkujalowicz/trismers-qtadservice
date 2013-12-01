#ifndef QADSERVICE_PLUGIN_H
#define QADSERVICE_PLUGIN_H

#include <QtGlobal>

#ifdef QADSERVICE_QT4
#include <QtDeclarative/QDeclarativeExtensionPlugin>
#else
#include <QtQml/QQmlExtensionPlugin>
#endif

class QAdServicePlugin: public
#ifdef QADSERVICE_QT4
       QDeclarativeExtensionPlugin
#else
       QQmlExtensionPlugin
#endif
{
    Q_OBJECT
#ifndef QADSERVICE_QT4
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface" FILE "plugin.json")
#endif
public:
    void registerTypes(const char *uri);
};

#endif // QADSERVICE_PLUGIN_H

