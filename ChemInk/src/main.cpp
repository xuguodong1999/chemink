#include "stdafx.h"
#include "scribblearea.h"
using namespace std;
#ifdef LINK_STATIC_QT5
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif // STATIC_BUILDBY_MSVC
int main(int argc, char* argv[]) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication::setOrganizationName(QObject::tr("南京大学计算机科学与技术系"));
	QApplication::setApplicationVersion(QObject::tr("版本-2019年12月"));
	QApplication::setApplicationName(QObject::tr("ChemInk 化学公式的手写分析与应用系统"));
	QApplication app(argc, argv);
	QTranslator translator;
	if (translator.load(":/Resources/qt_zh_CN.qm"))
		app.installTranslator(&translator);
	else {
		return throwErrorBox("Error Message", "Fail to load(\":/Resources/qt_zh_CN.qm\")!");
	}
	if (-1 == _putenv(QString("BABEL_DATADIR=" + QApplication::applicationDirPath() + "/data").toStdString().c_str()))
		return throwErrorBox("Error Message", "Fail to set BABEL_DATADIR!");
	ScribbleArea w;
	w.resize(getDesktopSize() / 1.5);
	w.show();
	return app.exec();
}
/*
version.lib
Netapi32.lib
userenv.lib
Dwmapi.lib
wtsapi32.lib
Wsock32.lib
Imm32.lib
Winmm.lib
ws2_32.lib
Qt5AccessibilitySupport.lib
Qt5Network.lib
Qt5Widgets.lib
Qt5Core.lib
Qt5Gui.lib
Qt5PlatformCompositorSupport.lib
Qt5EventDispatcherSupport.lib
Qt5FontDatabaseSupport.lib
Qt5ThemeSupport.lib
Qt5WindowsUIAutomationSupport.lib
Qt5OpenGL.lib
Qt5OpenGLExtensions.lib
opengl32.lib
glu32.lib
qtmain.lib
qwindows.lib
qgif.lib
qico.lib
qjpeg.lib
qtfreetype.lib
qtharfbuzz.lib
qtlibpng.lib
qsvgicon.lib
qicns.lib
qsvg.lib
qtga.lib
qtiff.lib
qwbmp.lib
qwebp.lib
qtpcre2.lib
*/