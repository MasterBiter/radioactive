#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDesktopWidget>
#include <QQmlComponent>
#include <QQmlContext>
#include <QQuickWindow>
#include <QDebug>
#include <QString>
#include "radioactivity.h"
#include "ttmath.h"
#include "radioactive.h"
typedef ttmath::Big<TTMATH_BITS(64), TTMATH_BITS(256)> big_shtuka;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    radioactive mainWind;
    mainWind.setGeometry(0,0,app.desktop()->screenGeometry().width(),app.desktop()->screenGeometry().height());
    mainWind.show();
    return app.exec();
}
