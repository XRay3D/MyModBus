#include "interface.h"
#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QLoggingCategory>
//#include <QQmlApplicationEngine>
//#include <QQmlContext>
#include <QSettings>
#include <QTranslator>

int main(int argc, char* argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    //    QLoggingCategory::setFilterRules(QStringLiteral("qt.modbus* = true"));

    QApplication app(argc, argv);

    QApplication::setApplicationName("FlowMeter");
    QApplication::setOrganizationName("Elemer");

    //    QFont font;
    //    font.setPixelSize(16);
    //    QApplication::setFont(font);

    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, app.applicationDirPath());

    static std::vector<std::unique_ptr<QTranslator>> translators;

    QDir dir(app.applicationDirPath().endsWith("/bin") ? app.applicationDirPath() + "/../" : app.applicationDirPath() + "/translations");

    for (const QString& str : dir.entryList(QStringList { "*ru_RU.qm" }, QDir::Files)) {
        translators.emplace_back(std::make_unique<QTranslator>());
        if (translators.back()->load(str, dir.path()))
            app.installTranslator(translators.back().get());
        else {
        }
    }

    [[maybe_unused]] I i;
    MainWindow w;
    w.show();

    //    if (0) {
    //        // QML
    //        QQmlApplicationEngine engine;
    //        const QUrl url(QStringLiteral("qrc:/main.qml"));
    //        QObject::connect(
    //            &engine, &QQmlApplicationEngine::objectCreated,
    //            &app, [url](QObject* obj, const QUrl& objUrl) {
    //                if (!obj && url == objUrl)
    //                    QCoreApplication::exit(-1);
    //            },
    //            Qt::QueuedConnection);
    //        engine.load(url);
    //        w.model()->setObjectName("regModel");
    //        engine.rootContext()->setContextProperty("regModel", w.model());
    //    }
    return app.exec();
}
