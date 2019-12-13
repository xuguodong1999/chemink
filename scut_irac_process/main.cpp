#define GEN_DATA

#ifdef GEN_DATA
#include "scutirac.h"
#include "generator.h"
#include <QString>
#include <QDebug>
#include <QTextCodec>
#include <QApplication>

/*测试单字符数据集生成效果*/
void testSingle();

/*以seed为随机种子生成一组完整的单字符数据集*/
void generateWithSeed(ScutIrac& s, int seed = 7);

/*以种子2、3、7生成三组单字符数据集*/
int generateSingle();

/*生成种子2、3、7生成三组单字符数据集的乱序文件名*/
int shuffleSingle();

/*生成三组目标检测数据集*/
int generateObjDetectionDataSet();

/*COUCH的顶层目录*/
const char* COUCH_DIR = "C:/DataSet/SCUT_IRAC/Couch";

/*生成的单字符数据集保存目录*/
const char* SAVE_DIR_SINGLE = "C:/DataSet/SCUT_IRAC/Chem78/";

/*生成的目标检测数据集保存目录*/
const char* SAVE_DIR_YOLO = "C:/DataSet/SCUT_IRAC/Yolov3-tiny-Chem76/";

/*精确标签文件*/
const char* f_mylabel = "./my_label.txt";

/*模糊标签文件*/
const char* f_scutlabel = "./scut_label.txt";

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);
	cout << "couch_dir = " << COUCH_DIR << endl;
	//return shuffleSingle();

	//return generateSingle();

	//return generateObjDetectionDataSet();
	cout << "enter event loop" << endl;
	return app.exec();
}

void testSingle() {
	ScutIrac datareader(COUCH_DIR);
	cout << datareader.imgs.size() << endl;
	cout << "done." << endl;
	int a; cin >> a;
	QPainterPath pth = Generator::getPath(datareader.imgs.at(a));
	QImage img;
	for (int i = 0; i < 100; i++) {
		img = Generator::getSingleImage(pth, 128, 1);
		img.save(QString("%1.jpg").arg(i));
	}
}

void generateWithSeed(ScutIrac& s, int seed) {
	srand(seed);
	int n = s.imgs.size();
	cout << "n=" << n << endl;
	QTextCodec* pCodec = QTextCodec::codecForName("gb2312");
	QImage img; QString savename;
	//int i = 962;
	for (int i = 0; i < n; i++) {
		img = Generator::getSingleImage(
			Generator::getPath(s.imgs.at(i)), 64, 1
		);
		savename = QString::number(i, 36) + "(" + s.mylabels.at(i).c_str() + ")"
			//+pCodec->toUnicode(s.scutlabels.at(i).c_str()) 
			+ ".jpg";
		img.save(SAVE_DIR_SINGLE + QString("%1/").arg(seed) + savename);
		//qDebug() << savename;
		//system("pause");
	}
}

int generateSingle() {
	ScutIrac::loadLabel();
	ScutIrac s(COUCH_DIR);
	generateWithSeed(s, 2);
	generateWithSeed(s, 3);
	generateWithSeed(s, 7);
	return 0;
}

int shuffleSingle() {
	ifstream ifsm1(string(SAVE_DIR_SINGLE) + "2.txt"),
		ifsm2(string(SAVE_DIR_SINGLE) + "3.txt"),
		ifsm3(string(SAVE_DIR_SINGLE) + "7.txt");
	string tmp;
	vector<string> res;
	ofstream ofsm(string(SAVE_DIR_SINGLE) + "train.list");
	while (getline(ifsm1, tmp)) {
		if (tmp.length() > 2)res.push_back(tmp);
	}
	while (getline(ifsm2, tmp)) {
		if (tmp.length() > 2)res.push_back(tmp);
	}
	while (getline(ifsm3, tmp)) {
		if (tmp.length() > 2)res.push_back(tmp);
	}
	for (int i = 0; i < 200; i++) {
		res.push_back("C:/DataSet/SCUT_IRAC/Chem78/(black).jpg");
		res.push_back("C:/DataSet/SCUT_IRAC/Chem78/(white).jpg");
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(res.begin(), res.end(), std::default_random_engine(seed));
	for (auto& i : res) {
		ofsm << i << endl;
	}
	ofsm.close(); ifsm1.close(); ifsm2.close(); ifsm3.close();
	return 0;
}

int generateObjDetectionDataSet() {
	ScutIrac::loadLabel(f_mylabel, f_scutlabel);
	ScutIrac s(COUCH_DIR);
	Generator::merge(s.imgs, s.intLabels, SAVE_DIR_YOLO);
	Generator::merge(s.imgs, s.intLabels, SAVE_DIR_YOLO);
	Generator::merge(s.imgs, s.intLabels, SAVE_DIR_YOLO);
	cout << "done." << endl;
	return 1;
}

#else
#include <QDir>
#include <QDebug>
#include <QImage>
#include <QFileInfo>
#include <QFileInfoList>
QImage img;
int FindFile(const QString& _filePath) {
	QDir dir(_filePath);
	if (!dir.exists()) {
		return -1;
	}
	dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
	dir.setSorting(QDir::DirsFirst);
	QFileInfoList list = dir.entryInfoList();
	if (list.size() < 1) {
		return -1;
	}
	int i = 0;
	do {
		QFileInfo fileInfo = list.at(i);
		bool bisDir = fileInfo.isDir();
		if (bisDir) {
			FindFile(fileInfo.filePath());
		}
		else {
			bool isJpg = fileInfo.fileName().endsWith(".jpg");
			if (isJpg) {
				img = QImage(fileInfo.filePath());
				img = img.convertToFormat(QImage::Format::Format_Grayscale8);
				img.save(fileInfo.filePath());
			}
			//qDebug() << fileInfo.filePath() << ":" << fileInfo.fileName();
		}
		i++;
	} while (i < list.size());
}
int main() {
	FindFile("E:/yoloChar/JPEGImages/");
	return 0;
}
#endif // GEN_DATA
