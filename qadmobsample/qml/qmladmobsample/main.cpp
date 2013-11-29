#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "qadmob_plugin.h"

Q_IMPORT_PLUGIN(QAdMobPlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    qobject_cast<QAdMobPlugin*>(qt_static_plugin_QAdMobPlugin().instance())->registerTypes("QtAdmob");

    viewer.setMainQmlFile(QStringLiteral("qml/qmladmobsample/main.qml"));
    viewer.showExpanded();

    return app.exec();
}
