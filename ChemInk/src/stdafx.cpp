#include "stdafx.h"

int throwErrorBox(const char* windowTitle, const char* message) {
	QErrorMessage dialog;
	dialog.setModal(true);
	dialog.setWindowTitle(QObject::tr(windowTitle));
	dialog.showMessage(QObject::tr(message));
	return dialog.exec();
}

QSize getDesktopSize() {
	return QApplication::desktop()->size();
}

int loadBabelPlugin() {
	QString babelDataDir = "BABEL_DATADIR=" + QApplication::applicationDirPath() + "/Resources/data";
	return _putenv(babelDataDir.toStdString().c_str());
	//QString babelLibDir = "BABEL_LIBDIR=" + QApplication::applicationDirPath() + "/data";
	//if (-1 == _putenv(babelLibDir.toStdString().c_str())) {
	//	return throwErrorBox(
	//		"Error Message",
	//		"Fail to set BABEL_LIBDIR!"
	//	);
	//}
}

/*
<AdditionalLibraryDirectories>
$(SolutionDir)ChemInk/third_party/openbabel-2.4.1/openbabel/lib;
</AdditionalLibraryDirectories>
*/
#ifdef LINK_BABEL2
#pragma comment(lib,"openbabel-2.lib")
#endif // LINK_BABEL2

/*
<AdditionalLibraryDirectories>
$(SolutionDir)ChemInk/third_party/opencv-4.1.1/x64/vc16/staticlib;
</AdditionalLibraryDirectories>
*/
#ifdef LINK_CV411
#pragma comment(lib,"zlib.lib")
#pragma comment(lib,"quirc.lib")
#pragma comment(lib,"IlmImf.lib")
#pragma comment(lib,"ade.lib")
#pragma comment(lib,"ippicvmt.lib")
#pragma comment(lib,"ippiw.lib")
#pragma comment(lib,"ittnotify.lib")
#pragma comment(lib,"libjasper.lib")
#pragma comment(lib,"libjpeg-turbo.lib")
#pragma comment(lib,"libpng.lib")
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"libtiff.lib")
#pragma comment(lib,"libwebp.lib")
#pragma comment(lib,"opencv_img_hash411.lib")
#pragma comment(lib,"opencv_world411.lib")
#endif // LINK_CV411

/*
<AdditionalLibraryDirectories>
$(QTDIR)\lib;$(QTDIR)\plugins\platforms;$(QTDIR)\plugins\imageformats;$(QTDIR)\plugins\platformthemes;$(QTDIR)\plugins\texttospeech;$(QTDIR)\plugins\sceneparsers;$(QTDIR)\plugins\position;$(QTDIR)\plugins\mediaservice;$(QTDIR)\plugins\generic;$(QTDIR)\plugins\styles;$(QTDIR)\plugins\scenegraph;$(QTDIR)\plugins\playlistformats;$(QTDIR)\plugins\gamepads;$(QTDIR)\plugins\sqldrivers;$(QTDIR)\plugins\renderplugins;$(QTDIR)\plugins\iconengines;$(QTDIR)\plugins\canbus;$(QTDIR)\plugins\sensors;$(QTDIR)\plugins\qmltooling;$(QTDIR)\plugins\geoservices;$(QTDIR)\plugins\bearer;$(QTDIR)\plugins\virtualkeyboard;$(QTDIR)\plugins\sensorgestures;$(QTDIR)\plugins\printsupport;$(QTDIR)\plugins\platforminputcontexts;$(QTDIR)\plugins\geometryloaders;$(QTDIR)\plugins\audio;
</AdditionalLibraryDirectories>
*/
#ifdef LINK_STATIC_QT5
#pragma comment(lib,"version.lib")
#pragma comment(lib,"Netapi32.lib")
#pragma comment(lib,"userenv.lib")
#pragma comment(lib,"Dwmapi.lib")
#pragma comment(lib,"wtsapi32.lib")
#pragma comment(lib,"Wsock32.lib")
#pragma comment(lib,"Imm32.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Qt5AccessibilitySupport.lib")
#pragma comment(lib,"Qt5Network.lib")
#pragma comment(lib,"Qt5Widgets.lib")
#pragma comment(lib,"Qt5Core.lib")
#pragma comment(lib,"Qt5Gui.lib")
#pragma comment(lib,"Qt5PlatformCompositorSupport.lib")
#pragma comment(lib,"Qt5EventDispatcherSupport.lib")
#pragma comment(lib,"Qt5FontDatabaseSupport.lib")
#pragma comment(lib,"Qt5ThemeSupport.lib")
#pragma comment(lib,"Qt5WindowsUIAutomationSupport.lib")
#pragma comment(lib,"Qt5OpenGL.lib")
#pragma comment(lib,"Qt5OpenGLExtensions.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"qtpcre2.lib")
#pragma comment(lib,"qtmain.lib")
#pragma comment(lib,"qwindows.lib")
#pragma comment(lib,"qgif.lib")
#pragma comment(lib,"qico.lib")
#pragma comment(lib,"qjpeg.lib")
#pragma comment(lib,"qtfreetype.lib")
#pragma comment(lib,"qtharfbuzz.lib")
#pragma comment(lib,"qtlibpng.lib")
#pragma comment(lib,"qsvgicon.lib")
#pragma comment(lib,"qicns.lib")
#pragma comment(lib,"qsvg.lib")
#pragma comment(lib,"qtga.lib")
#pragma comment(lib,"qtiff.lib")
#pragma comment(lib,"qwbmp.lib")
#pragma comment(lib,"qwebp.lib")
#endif // LINK_STATIC_QT5

#ifdef ANDROID_BUILDBY_WIN_QMAKE
bool requestPermission(QString moduleName) {
	QtAndroid::PermissionResult r = QtAndroid::checkPermission("android.permission." + moduleName);
	if (r == QtAndroid::PermissionResult::Denied) {
		QtAndroid::requestPermissionsSync(QStringList() << "android.permission." + moduleName);
		r = QtAndroid::checkPermission("android.permission." + moduleName);
		if (r == QtAndroid::PermissionResult::Denied) {
			return false;
		}
	}
	return true;
}
#endif // ANDROID_BUILDBY_WIN_QMAKE

//  非移动端允许使用gl库
#ifndef ANDROID_BUILDBY_WIN_QMAKE
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#endif // !ANDROID_BUILDBY_WIN_QMAKE
