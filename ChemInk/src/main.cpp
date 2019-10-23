//  #define MAKE_ico			//  制作背景图
//  #define TEST_glmol			//  渲染化合物望望
//  #define TEST_forcefield		//  测试输入smiles字符串输出坐标文件
//  #define TEST_fdeepcnn		//  测试fdeep推理keras cnn模型
//  #define TEST_opencvyolov3	//  测试据说是最快的cpu推理库//  实测跟fdeep差不多
//  #define TEST_corrector		//  测试输入纠错-(不是)基于编辑距离和简单的概率模型
  #define TEST_app				//  测试最终应用

#define STATICBUILD

#ifdef TEST_app
#include "writtingpanel.h"
#include <QApplication>

#ifdef STATICBUILD
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#endif //! STATICBUILD

int main(int argc, char* argv[]) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);

	WrittingPanel w;
	w.showMaximized();

	return a.exec();
}
#elif defined(TEST_corrector)
#include "spellcorrector.h"
#include<iostream>
using namespace std;
int main() {
	SpellCorrector spellcorrector;
	string input;
	while (getline(cin, input)) {
		spellcorrector.getBestResult();
	}

	return 0;
}

#elif defined(TEST_opencvyolov3)
#include "opencvyolov3.h"
#include <QImage>
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QThreadPool>
#include <QThread>
#include <QRunnable>
#include <QDebug>
#include <Windows.h>
//#include <unistd.h>
using namespace std;
int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);
	qDebug() << "main thread: " << QThread::currentThreadId();
	QThreadPool threadpool;
	qDebug() << threadpool.maxThreadCount() << threadpool.expiryTimeout();
	qDebug() << threadpool.activeThreadCount();
	//  加载负载
	int n = 10, w = 416, h = 416; //cout << "input loop times: "; cin >> n;
	vector<QImage> reader;
	reader.resize(n);
	reader.at(0).load("Resources/images/testcase.png");
	reader.at(0) = reader.at(0).scaled(w, h);
	for (int i = 1; i < n; i++) reader.at(i) = reader.at(0);
	OpencvYolov3 yolov3;
	yolov3.setWeightsfile("Resources/weights/chardetector-yolov3-tiny_226000.weights", "Resources/weights/chardetector-yolov3-tiny.cfg");
	//  设置模型
	yolov3.setOutputDimension(76);
	vector<DetectionObject> boxes;
	yolov3.setInOut(&reader.at(0), &boxes);
	qDebug() << reader.at(0).width() << reader.at(0).height();
	yolov3.setInputSize(w, h);

	cout << " //  开线程加载模型" << endl;
	yolov3.setWorkIndex(LOAD);
	threadpool.start(&yolov3);
	cout << threadpool.activeThreadCount();
	threadpool.waitForDone();
	cout << threadpool.activeThreadCount();
	cout << "//  开线程做推理" << endl;
	yolov3.setWorkIndex(PREDICT);
	threadpool.start(&yolov3);
	cout << "//  线程已经在工作" << endl;
	qDebug() << threadpool.activeThreadCount();
	for (int i = 0; i < 3; i++) {
		cout << "滴答..";
		Sleep(1000);
	}
	cout << "主线中途睡了3秒" << endl;
	time_t start = clock();
	yolov3.setInOut(&reader.at(1), &boxes);
	for (int fuck = 0; fuck < n; fuck++) {
		threadpool.start(&yolov3);
		threadpool.waitForDone();
	}
	cout << (clock() - start) << "ms" << endl;
	cout << "线程池里没东西了" << endl;
	qDebug() << threadpool.activeThreadCount();
	if (!yolov3.isWorking())
		cout << "任务完成" << endl;
	for (size_t j = 0; j < boxes.size(); j++)
		qDebug() << boxes.at(j) << "," << boxes.at(j).class_id << "," << boxes.at(j).confidence;
	system("pause");
	return 0;
}

#elif defined(TEST_forcefield)
#include "forcefieldcaller.h"
#include <iostream>
#include <string>
#include <QThreadPool>
#include <QThread>
#include <QRunnable>
#include <QDebug>
using namespace std;
int main(int argc, char* argv[]) {
	QThreadPool t;
	ForceFieldCaller ffc; ffc.setOutputFormat("mol2");
	char a[1000]; string res;
	cout << "输入标准SMILES分子式：";
	ffc.setWorkIndex(LOAD);
	t.start(&ffc);
	while (cin >> a) {
		ffc.smiles2coordinates(a, res);
		cout << res << endl << "输入标准SMILES分子式：";
	}
	return 0;
}
#elif defined(TEST_fdeepcnn)
#include "fdeepcnn.h"
#include <QImage>
#include <iostream>
#include <QDebug>
#include <QCoreApplication>
#include <QThreadPool>
#include <QThread>
#include <QRunnable>
#include <QDebug>
#include <unistd.h>
using namespace std;
int main(int argc, char* argv[]) {
	QCoreApplication a(argc, argv);
	qDebug() << "main thread: " << QThread::currentThreadId();
	QThreadPool threadpool;
	qDebug() << threadpool.maxThreadCount() << threadpool.expiryTimeout();
	qDebug() << threadpool.activeThreadCount();
	//  加载负载
	int n = 10, w = 32; //cout << "input loop times: "; cin >> n;
	vector<QImage> reader;
	reader.resize(n);
	reader.at(0).load("Resources/images/testcase.png");
	reader.at(0) = reader.at(0).scaled(w, w);
	for (int i = 1; i < n; i++) reader.at(i) = reader.at(0);
	FdeepCNN cnn;
	cnn.setWeightsfile("Resources/weights/densenet-chdsbw.json");
	//  设置模型
	cnn.setOutputDimension(78);
	vector<vector<float>> topk;
	cnn.setInOut(&reader, &topk);
	cnn.setAutoDelete(false);
	//  开线程加载模型
	cnn.setWorkIndex(LOAD);
	threadpool.start(&cnn);
	qDebug() << threadpool.activeThreadCount();
	threadpool.waitForDone();
	qDebug() << threadpool.activeThreadCount();
	//  开线程做推理
	cnn.setWorkIndex(LOAD);
	cnn.setWorkIndex(PREDICT);
	threadpool.start(&cnn);
	threadpool.waitForDone();
	threadpool.start(&cnn);
	//  线程已经在工作
	qDebug() << threadpool.activeThreadCount();
	for (int i = 0; i < 3; i++) {
		cout << "滴答..";
		sleep(1);
	}
	cout << "主线中途睡了3秒" << endl;
	threadpool.waitForDone();
	cout << "线程池里没东西了" << endl;
	qDebug() << threadpool.activeThreadCount();
	if (!cnn.isWorking())
		cout << "任务完成" << endl;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < topk.at(i).size(); j++)
			cout << topk.at(i).at(j) << "\t";
		cout << endl << endl;
	}
	return 0;
}
#elif defined(TEST_glmol)
#include "glmolarea.h"
#include <QApplication>
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
int main(int argc, char* argv[]) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	GlMolArea w(
		"@<TRIPOS>MOLECULE\n\
*****\n\
 18 18 0 0 0\n\
SMALL\n\
GASTEIGER\n\
\n\
@<TRIPOS>ATOM\n\
      1 C          -1.4184    0.3270   -0.2373 C.3     1  UNL1       -0.0531\n\
      2 C          -0.9909   -1.0652    0.2498 C.3     1  UNL1       -0.0531\n\
      3 C           0.4257   -1.3918   -0.2452 C.3     1  UNL1       -0.0531\n\
      4 C           1.4196   -0.3277    0.2427 C.3     1  UNL1       -0.0531\n\
      5 C           0.9923    1.0646   -0.2444 C.3     1  UNL1       -0.0531\n\
      6 C          -0.4244    1.3912    0.2504 C.3     1  UNL1       -0.0531\n\
      7 H          -2.4351    0.5614    0.1445 H       1  UNL1        0.0265\n\
      8 H          -1.4535    0.3354   -1.3492 H       1  UNL1        0.0265\n\
      9 H          -1.7045   -1.8290   -0.1268 H       1  UNL1        0.0265\n\
     10 H          -1.0092   -1.0899    1.3619 H       1  UNL1        0.0265\n\
     11 H           0.4314   -1.4213   -1.3573 H       1  UNL1        0.0265\n\
     12 H           0.7326   -2.3912    0.1309 H       1  UNL1        0.0265\n\
     13 H           2.4364   -0.5621   -0.1390 H       1  UNL1        0.0265\n\
     14 H           1.4546   -0.3361    1.3546 H       1  UNL1        0.0265\n\
     15 H           1.0107    1.0894   -1.3565 H       1  UNL1        0.0265\n\
     16 H           1.7058    1.8284    0.1324 H       1  UNL1        0.0265\n\
     17 H          -0.7313    2.3906   -0.1259 H       1  UNL1        0.0265\n\
     18 H          -0.4302    1.4210    1.3625 H       1  UNL1        0.0265\n\
@<TRIPOS>BOND\n\
     1     1     2    1\n\
     2     2     3    1\n\
     3     3     4    1\n\
     4     4     5    1\n\
     5     5     6    1\n\
     6     1     6    1\n\
     7     1     7    1\n\
     8     1     8    1\n\
     9     2     9    1\n\
    10     2    10    1\n\
    11     3    11    1\n\
    12     3    12    1\n\
    13     4    13    1\n\
    14     4    14    1\n\
    15     5    15    1\n\
    16     5    16    1\n\
    17     6    17    1\n\
    18     6    18    1\n", MOL2);
	w.show();
	return a.exec();
}
#elif defined(MAKE_ico)//  可以用来做透明背景
#include <QImage>
#include <QDebug>
using namespace std;
#include <QtCore/QtPlugin>
#include <iostream>
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
int main() {
	string infile, outfile; int x; bool allowIn = 0;
	while (!allowIn) {
		cout << "输入目标文件名："; cin >> infile;
		cout << "输入保存文件名："; cin >> outfile;
		cout << "输入要透明化的颜色值："; cin >> x;
		cout << "确认转换吗：1/0)"; cin >> allowIn;
	}
	QImage image, is; image.load(infile.c_str());
	uchar* buffer = image.bits();
	int w = image.width(); int h = image.height(); int pixCounter = w * h;
	QColor c1 = QColor(255, 222, 233), c2 = Qt::black, c3 = Qt::white;
	is = image; //is.fill(c1);
	is.convertTo(QImage::Format_ARGB32);
	for (int i = 0; i < pixCounter; i++)
		if (buffer[4 * i] == x) {
			is.setPixelColor((i) % w, (i) / w, Qt::transparent);
			/*for (int j = 1; j < 6; j++) {
				is.setPixelColor((i) % w + j, (i) / w, c2);
				is.setPixelColor((i) % w, (i) / w + j, c2);
				is.setPixelColor((i) % w, (i) / w - j, c2);
				is.setPixelColor((i) % w - j, (i) / w, c2);
			}*/
		}
	is.save(outfile.c_str()); return 0;
}
#endif
