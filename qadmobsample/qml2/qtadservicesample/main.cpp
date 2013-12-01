#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"

#include "qadservice_plugin.h"
#include "qadservice.h"

Q_IMPORT_PLUGIN(QAdMobPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    qobject_cast<QAdMobPlugin*>(qt_static_plugin_QAdMobPlugin().instance())->registerTypes("QtAdService");
    viewer.setMainQmlFile(QStringLiteral("qml/qtadservicesample/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
