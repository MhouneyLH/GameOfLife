#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "GameOfLifeModel.h"
#include "QmlAdapter.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QmlAdapter adapter;
    engine.rootContext()->setContextProperty("_qmlAdapter", &adapter);

    qmlRegisterType<GameOfLifeModel>("GameOfLifeModel", 1, 0, "GameOfLifeModel");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
