#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>

#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "TecnaIpDevManager_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    QQuickView view;
        view.setTitle("TECNA IP Config");
        // view.engine()->addImportPath("qrc:/qml/imports");
        view.setSource(QUrl("qrc:/qml/content/MainScreen.qml"));
        if (!view.errors().isEmpty())
            return -1;
        view.show();


//    QQuickView view;
//        view.setTitle("TECNA IP Config");
//        // view.engine()->addImportPath("qrc:/qml/imports");
//        // view.setSource(QUrl("qrc:/qml/content/MainScreen.qml"));
//        view.setSource(QUrl("qrc:/TecnaIpDevManager/main.qml"));
//        if (!view.errors().isEmpty())
//            return -1;
//        view.show();
    return app.exec();
}
