#include "animal.h"
#include "animalmodel.h"
#include <QAbstractListModel>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AnimalModel model;
    model.initDb();
    //    model.m_insert(0, false, "Wolf", "Medium");
    //    model.m_insert(1, true, "Polar bear", "Large");
    //    model.m_insert(2, false, "Quoll", "Small");

    engine.rootContext()->setContextProperty("myModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
