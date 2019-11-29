#include "stdafx.h"
#include "mainwindow.h"
#include "forcefield.h"
#include "molecule.h"
#include "glumolwindow.h"
#include "scribblearea.h"
using namespace std;
/*
应用开发备忘录：
1、用openbabel和opencv库做出程序原型
2、本地化除矩阵库eigen外的所有第三方库
3、能用标准库就用标准库，不行就用Qt库，再不行才用第三方库
4、UI采用桌面一套(qwidget)、移动端一套(qml)的方案，桌面应用考虑触屏，不考虑竖屏
5、UI设计对标2019版的word和ppt
6、化学专业对标openchemistry全家桶
7、画板对标概念画板
8、多思考，少重构，一旦有重构的必要，立即重构，不要拖
*/
#ifdef LINK_STATIC_QT
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif // STATIC_BUILDBY_MSVC

//  #define TEST_BABEL
//  #define TEST_GL
//  #define TEST_APP
#define TEST_SCRIPT
//  #define TEST_YOLO
//  #define TEST_VBO //  测试vbo模式 fail
//  #define TEST_EIGEN
//  #define TEST_QCOMPRESS //测试qt自带的压缩

int main(int argc, char* argv[]) {
	//qDebug() << QStyleFactory::keys();
	//QApplication::setStyle(QStyleFactory::create("windowsvista"));
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication::setOrganizationName(QObject::tr("南京大学计算机科学与技术系"));
	QApplication::setApplicationVersion(QObject::tr("版本-2019年11月"));
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

#ifdef TEST_SCRIPT
	ScribbleArea w;
	w.resize(getDesktopSize() / 1.5);
	w.show();
#endif // TEST_SCRIPT
#ifdef TEST_APP
	MainWindow w;
	w.resize(getDesktopSize() / 1.5);
	w.show();
#elif defined(TEST_BABEL)
#pragma execution_character_set("GB2312")
	string input;
	string coor;
	ForceField ff;
	cout << "输入：";
	while (cin >> input) {
		coor = ff.getCoordinate(input.c_str(), "smiles", "pdb");
		cout << "坐标：\n" << coor << endl;
		cout << "能量：" << ff.getEnergy(coor.c_str(), "pdb") << endl;
		cout << "输入：";
	}
#elif defined(TEST_GL)
#pragma execution_character_set("GB2312")
	string input;
	string coor;
	ForceField ff;
	Molecule mol;
	char* format = "MOL2";
	input = "CC";
	cout << "输入SMILES字符串："; getline(cin, input);
	coor = ff.getCoordinate(input.c_str(), "SMILES", format);
	std::cout << "坐标：\n" << coor << endl;
	mol.load(coor.c_str(), format);
	mol.display();
	std::cout << "mol.atoms.size()=" << mol.getAtomSize() << endl;
	std::cout << "mol.bonds.size()=" << mol.getBondSize() << endl;
	std::cout << "能量：" << ff.getEnergy(coor.c_str(), format) << endl;
	GluMolWindow w;
	w.createMol3d(mol);
	w.resize(getDesktopSize() / 2.5);
	w.show();
	std::cout << "进入事件循环..." << endl;
	//}
#elif defined(TEST_VBO)
	//#pragma execution_character_set("GB2312")
	std::cout << "Hello, vbo!" << endl;


	std::cout << "Bye, vbo!" << endl;
#elif defined(TEST_EIGEN)
	Eigen::Vector3d v1(10, 0, 0), v2(0, 10, 0), v3(10, 10, 0);
	cout << v1.cross(v2);
	cout << v2.cross(v1) << endl;
	cout << sqrt(v3.dot(v3)) / sqrt(2) << endl;
	cout << v3.norm() / sqrt(2);
#endif
#ifdef TEST_QCOMPRESS
	QFile file("E:/chardetector-yolov3-tiny_226000.weights");
	if (file.exists()) {
		qDebug() << "压缩前文件大小:" << file.size();
		bool ok = file.open(QIODevice::ReadOnly);
		if (ok) {
			QByteArray buffer = file.readAll();
			//压缩
			buffer = qCompress(buffer, 2);//使用默认的zlib压缩
			qDebug() << "压缩后字节大小:" << buffer.size();
			QFile writeFile("E:/TMP.zip");
			ok = writeFile.open(QIODevice::WriteOnly);
			if (ok) {
				//解压缩
				//buffer = qUncompress(buffer);
				qDebug() << "解压缩后字节大小:" << buffer.size();
				writeFile.write(buffer);
				qDebug() << "压缩后文件大小:" << writeFile.size();
			}
		}
	}
	return 0;
#endif // TEST_QCOMPRESS
	return app.exec();
}
