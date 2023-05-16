#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <QLocale>
#include <QTranslator>
#include "QQmlContext"

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QStringListModel>

#include "tp_view_object_helper.h"
#include "seriemodel.h"


int main(int argc, char *argv[])
{

/**
 * UNCOMMENT IF YOU WANT TRANSLATOR
//    QTranslator translator;
//    const QStringList uiLanguages = QLocale::system().uiLanguages();
//    for (const QString &locale : uiLanguages) {
//        const QString baseName = "TecnaIpDevManager_" + QLocale(locale).name();
//        if (translator.load(":/i18n/" + baseName)) {
//            app.installTranslator(&translator);
//            break;
//        }
//    }

*/
        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;
        const QUrl url(u"qrc:/qml/content/MainScreen.qml"_qs);
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

        TP_VIEW_OBJECT_HELPER tp_view_object_helper;
        engine.rootContext()->setContextProperty("tp_view_object_helper", &tp_view_object_helper);

        QStringListModel *if_model = new QStringListModel();
        engine.rootContext()->setContextProperty("tp_if_combo_model", if_model);
        tp_view_object_helper.getModel(if_model);

        QStringListModel *ip_model = new QStringListModel();
        engine.rootContext()->setContextProperty("tp_ip_combo_model", ip_model);
        tp_view_object_helper.getIPModel(ip_model);

        SerieModel *serieModel = new SerieModel(&tp_view_object_helper);
        engine.rootContext()->setContextProperty("serieModel", serieModel);

        engine.load(url);


    return app.exec();
}
