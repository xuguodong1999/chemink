#include "chemink.h"
#include "scribblearea.h"
#include <QApplication>
#include <QTranslator>

using namespace std;

int main(int argc, char *argv[]) {
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QApplication::setOrganizationName(QObject::tr("南京大学计算机科学与技术系"));
    QApplication::setApplicationVersion(QObject::tr("2019.12.13"));
    QApplication::setApplicationName(QObject::tr("ChemInk 化学公式的手写分析与应用系统"));
    QApplication app(argc, argv);
    QTranslator translator;
    if (translator.load(":/qt_zh_CN.qm"))
        app.installTranslator(&translator);
    else {
        return throwErrorBox(
                "Error Message", "Fail to load(\":/qt_zh_CN.qm\")!");
    }
    auto obDataDir = QApplication::applicationDirPath() + "/chemink-data";
    releaseQrcFile(":/obabel/", obDataDir);
    if (-1 == _putenv(("BABEL_DATADIR=" + obDataDir).toStdString().c_str()))
        return throwErrorBox("Error Message", "Fail to set BABEL_DATADIR!");
    ScribbleArea w;
    w.resize(getDesktopSize() / 1.5);
    w.show();
    return app.exec();
}
