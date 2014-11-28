#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    app->setApplicationName ("9News");
    app->setOrganizationName ("Stars");
    app->setApplicationVersion ("0.0.1");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
#ifdef HARMATTAN_BOOSTER
    viewer.setMainQmlFile(QLatin1String("qml/meego/main.qml"));
#else
    viewer.setMainQmlFile(QLatin1String("qml/symbian/main.qml"));
#endif
    viewer.showExpanded();

    return app->exec();
}
