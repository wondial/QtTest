#include <QAbstractListModel>
#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "animal.h"
#include "animalmodel.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    AnimalModel model;
    model.m_insert(0, false, "Wolf", "Medium");
    model.m_insert(1, true, "Polar bear", "Large");
    model.m_insert(2, false, "Quoll", "Small");

//    UserListModel userlistModel;
//    QString mac = "12:3e:3w:4r:33";
//    userlistModel.addUser(User(mac));
//    userlistModel.addUser(User(mac));
//    userlistModel.addUser(User(mac));
//    userlistModel.addUser(User(mac));
//    QModelIndex ix = userlistModel.index(1, 0);
//    QVariant v = userlistModel.data(ix, UserListModel::macRole);
//    qDebug() << "===========" << v.toString();

    engine.rootContext()->setContextProperty("myModel", &model);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
