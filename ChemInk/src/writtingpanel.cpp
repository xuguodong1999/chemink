#include "writtingpanel.h"
#include "ui_writtingpanel.h"
#include <iostream>
#include <QSplashScreen>
#include <QDebug>
#include <QPainter>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

//#include <synchapi.h>
using namespace std;

static inline QRect extend(const QRect& r1, const QRect& r2) {
	int xmin = r1.x() < r2.x() ? r1.x() : r2.x();
	int ymin = r1.y() < r2.y() ? r1.y() : r2.y();
	int xmax = r1.bottomRight().x() > r2.bottomRight().x() ? r1.bottomRight().x() : r2.bottomRight().x();
	int ymax = r1.bottomRight().y() > r2.bottomRight().y() ? r1.bottomRight().y() : r2.bottomRight().y();
	return QRect(QPoint(xmin, ymin), QPoint(xmax, ymax));
}

static inline string resCode2Block(int resCode) {
	switch (resCode) {
	case  0:case  26:
		return "3";
	case  1:
		return "4";
	case  2:
		return "7";
	case  3:
		return "8";
	case  4:
		return "A";
	case  5:
		return "B++";
	case  6:
		return "D";
	case  7:
		return "E++";
	case  8:
		return "F";
	case  9:
		return "G";
	case  10:case 31:
		return "H";
	case  11:
		return "J";
	case  12:
		return "L";
	case  13:
		return "N";
	case  14:
		return "Q";
	case  15:
		return "R";
	case  16:
		return "T++";
	case  17:
		return "a+";
	case  18:
		return "at";
	case  19:
		return "b+";
	case  20:
		return "black";
	case  21:
		return "c+";
	case  22:
		return "dd";
	case  23:
		return "div";
	case  24:
		return "down";
	case  25:
		return "ee";
		//case  26:
		//	return "epsilon+";
	case  27:
		return "equal";
	case  28:
		return "eta";
	case  29:
		return "fai+";
	case  30:
		return "ff";
		//case  31:
		//	return "hash";
	case  32:
		return "hh";
	case  33:
		return "ii";
	case  34:
		return "jj";
	case  35:
		return "k+";
	case  36:
		return "l+";
	case  37:
		return "lambda";
	case  38:
		return "lbb";
	case  39:
		return "left";
	case  40:
		return "lequal";
	case  41:
		return "less";
	case  42:
		return "lrb";
	case  43:
		return "lsb";
	case  44:
		return "m+";
	case  45:
		return "mequal";
	case  46:
		return "minus";
	case  47:
		return "mod";
	case  48:
		return "more";
	case  49:
		return "mul";
	case  50:
		return "nequal";
	case  51:
		return "nn";
	case  52:
		return "o+";
	case  53:
		return "omega";
	case  54:
		return "p+";
	case  55:
		return "pai+";
	case  56:
		return "plus";
	case  57:
		return "q+";
	case  58:
		return "rbb";
	case  59:
		return "right";
	case  60:
		return "rmb";
	case  61:
		return "rr";
	case  62:
		return "rrb";
	case  63:
		return "rsb";
	case  64:
		return "s+";
	case  65:
		return "sigma";
	case  66:
		return "theta";
	case  67:
		return "tt";
	case  68:
		return "u+";
	case  69:
		return "up";
	case  70:
		return "v+";
	case  71:
		return "w+";
	case  72:
		return "what";
	case  73:
		return "white";
	case  74:
		return "x+";
	case  75:
		return "xor";
	case  76:
		return "y+";
	case  77:
		return "z+";
	default:
		return "[Error: Unknow resCode.]";
	}
}

static vector<int> subindexes;

static bool boxCmp(const QRect& a, const QRect& b) {
	return a.center().x() < b.center().x();
}

static inline void sortFloatWithSubIndex(vector<float>& arr) {
	size_t n = arr.size();
	subindexes.resize(n);
	for (size_t i = 0; i < n; i++)
		subindexes[i] = i;
	for (size_t i = 0; i < n - 1; i++) {
		size_t index = i;
		for (size_t j = i + 1; j < n; j++) {
			if (arr[index] < arr[j])
				index = j;
		}
		if (index != i) {
			std::swap(arr[index], arr[i]);
			std::swap(subindexes[index], subindexes[i]);
		}
	}
}

const vector<string> fileSrc = {
	":/Resources/openbabel/rigid-fragments.txt",
	":/Resources/openbabel/rigid-fragments-index.txt",
	":/Resources/openbabel/ring-fragments.txt",
	":/Resources/openbabel/UFF.prm",
	":/Resources/weights/chardetector-yolov3-tiny.cfg",
	":/Resources/weights/densenet-chdsbw.json",

	":/Resources/weights/1.dat",
	":/Resources/weights/2.dat",
	":/Resources/weights/3.dat",
	":/Resources/weights/4.dat",
};

const vector<string> fileOut = {
	"./rigid-fragments.txt",
	"./rigid-fragments-index.txt",
	"./ring-fragments.txt",
	"./UFF.prm",
	"./Resources/weights/chardetector-yolov3-tiny.cfg",
	"./Resources/weights/densenet-chdsbw.json",

	"./Resources/weights/chardetector-yolov3-tiny_226000.weights",
};
/*const vector<string> fileOut = {
	"./rigid-fragments.txt",
	"./rigid-fragments-index.txt",
	"./ring-fragments.txt",
	"./UFF.prm",
	"/storage/emulated/0/Download/Resources/weights/chardetector-yolov3-tiny.cfg",
	"/storage/emulated/0/Download/Resources/weights/densenet-chdsbw.json",

	"/storage/emulated/0/Download/Resources/weights/chardetector-yolov3-tiny_226000.weights",
};*/

WrittingPanel::WrittingPanel(QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::WrittingPanel) {

	////////////////  启动动画，资源加载完成后消失
	QSplashScreen splash(QPixmap(":/Resources/images/loading.png").scaled(450, 300));
	splash.setDisabled(true);
	splash.show();
	QFont font; font.setPixelSize(15); font.setBold(true); font.setItalic(true);
	splash.setFont(font);
	splash.showMessage(
		QObject::tr("Resourses loading......"), Qt::AlignTop | Qt::AlignRight, Qt::red
	);
	//////////////  启动动画，资源加载完成后消失

	hide();
	ui->setupUi(this);
	//  文件尽量用Qt提供的文件系统读取，
	//  第三方库openbabel的插件加载方式不知道如何更改，
	//  采用把资源文件释放到可执行目录下的方法进行打包
	//  权重文件太大，zip压缩效果不明显
	//  这个只能随可执行文件一起
	//  安卓系统可以把这个文件扔到android/assert文件夹下自动打包进app


	//  向可执行文件所在的目录释放资源文件
	//  依次检查所有输出文件
	QDir dir;
	if (!dir.exists("./Resources/weights")) {
		if (!dir.mkpath("./Resources/weights")) {
			cout << "缺少文件读写权限" << endl;
			exit(-1);
		}
	}

	/*if (!dir.exists("/storage/emulated/0/Download/Resources/weights")) {
		if (!dir.mkpath("/storage/emulated/0/Download/Resources/weights")) {
			cout << "缺少文件读写权限" << endl;
			exit(-1);
		}
	}*/

	for (size_t i = 0; i < fileOut.size(); i++) {
		QFileInfo info(fileOut.at(i).c_str());
		if (info.exists()) {
			continue;
		}
		else {
			if (fileOut.at(i).find(".weights") == std::string::npos) {
				QFile freader(fileSrc.at(i).c_str()),
					fwritter(fileOut.at(i).c_str());
				freader.open(QIODevice::ReadOnly);
				fwritter.open(QIODevice::WriteOnly);
				fwritter.write(freader.readAll());
				freader.close();
				fwritter.close();
			}
			else {
				QFile freader1(fileSrc.at(i).c_str()),
					freader2(fileSrc.at(i + 1).c_str()),
					freader3(fileSrc.at(i + 2).c_str()),
					freader4(fileSrc.at(i + 3).c_str()),
					fwritter(fileOut.at(i).c_str());
				freader1.open(QIODevice::ReadOnly);
				freader2.open(QIODevice::ReadOnly);
				freader3.open(QIODevice::ReadOnly);
				freader4.open(QIODevice::ReadOnly);
				fwritter.open(QIODevice::WriteOnly);
				fwritter.write(freader1.readAll());
				fwritter.write(freader2.readAll());
				fwritter.write(freader3.readAll());
				fwritter.write(freader4.readAll());
				freader1.close();
				freader2.close();
				freader3.close();
				freader4.close();
				fwritter.close();
			}
		}
	}

	pencilIcon.load(":/Resources/images/pencil.png");
	rubberIcon.load(":/Resources/images/rubber.png");

	scribblearea = new ScribbleArea(ui->widget);
	scribblearea->loadStyle(BLACKBOARD);
	curScript = scribblearea->getScript();

	threadpool = new QThreadPool();

	detector = new OpencvYolov3();

	/*detector->setWeightsfile(
		"/storage/emulated/0/Download/Resources/weights/chardetector-yolov3-tiny_226000.weights",
		"/storage/emulated/0/Download/Resources/weights/chardetector-yolov3-tiny.cfg"
	);*/
	detector->setWeightsfile(
		"./Resources/weights/chardetector-yolov3-tiny_226000.weights",
		"./Resources/weights/chardetector-yolov3-tiny.cfg"
	);
	detector->setOutputDimension(76);
	detector->setInOut(&detectorImg, &boxes);
	detector->setThreahhold(0.2, 0.5);
	detector->setWorkIndex(LOAD);

	classifier = new FdeepCNN();
	//classifier->setWeightsfile("/storage/emulated/0/Download/Resources/weights/densenet-chdsbw.json");
	classifier->setWeightsfile("./Resources/weights/densenet-chdsbw.json");
	classifier->setOutputDimension(78);
	classifier->setInOut(&classifierImgs, &topks);
	classifier->setWorkIndex(LOAD);

	forcefieldcaller = new ForceFieldCaller();
	forcefieldcaller->setWorkIndex(LOAD);

	chembalancer = new ChemBalancer();

	alkanetopo = new AlkaneTopo();

	spellcorrector = new SpellCorrector();

	_connect();

	//  启动三个资源加载线程
	threadpool->start(detector);
	//threadpool->waitForDone();
	//  system("pause");
	//  exit(-1);
	threadpool->start(classifier);

	threadpool->start(forcefieldcaller);
	// 阻塞主线程，直到资源加载完成
	threadpool->waitForDone();
	//  Sleep(2000);

	show();
}

void WrittingPanel::_connect() {
	connect(//  同步笔迹数据
		scribblearea, &ScribbleArea::sendStrokeData,
		this, &WrittingPanel::acceptStrokeData
	);


	connect(//  书写/橡皮模式切换
		ui->tbtn_rubber, &QToolButton::clicked,
		scribblearea, &ScribbleArea::acceptRubber
	);

	connect(//  书写/橡皮模式切换
		ui->tbtn_rubber, &QToolButton::clicked,
		this, &WrittingPanel::acceptRubber
	);

	connect(//  清空屏幕，转储笔画
		ui->tbtn_cls, &QToolButton::clicked,
		this, &WrittingPanel::cleanCurScript
	);

	connect(//  退出程序
		ui->tbtn_exit, &QToolButton::clicked,
		this, &WrittingPanel::close
	);

	connect(//  撤销上一步
		ui->tbtn_undo, &QToolButton::clicked,
		scribblearea, &ScribbleArea::undo
	);

	connect(//  调试专用
		ui->tbtn_set, &QToolButton::clicked,
		this, [=](bool check) {
			if (curScript->size() == 0)
				return;
			setDetectorImg();
			getBoxes();
			syncBoxes();
			preciseClassify();
		}
	);

	//connect(//  调试专用
	//	scribblearea, &ScribbleArea::askForRecognize,
	//	this, [=]() {
	//		if (curScript->size() == 0 || detector->isWorking() || classifier->isWorking())
	//			return;
	//		setDetectorImg();
	//		getBoxes();
	//		syncBoxes();
	//		preciseClassify();
	//	}
	//);
}

void WrittingPanel::cleanCurScript() {
	scribblearea->clear();
}

void WrittingPanel::acceptRubber(bool _isPen) {
	if (_isPen) {
		ui->tbtn_rubber->setStyleSheet(QString(
			"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
			"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
			"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
		).arg("pencil"));
	}
	else {
		ui->tbtn_rubber->setStyleSheet(QString(
			"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
			"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
			"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
		).arg("rubber"));
	}
}

WrittingPanel::~WrittingPanel() {
	for (auto& i : historyScripts) {
		if (i) {
			delete i;
			i = nullptr;
		}
	}
	if (scribblearea)
		delete scribblearea;
	if (detector)
		delete detector;
	if (classifier)
		delete classifier;
	if (threadpool)
		delete threadpool;
	if (chembalancer)
		delete chembalancer;
	if (alkanetopo)
		delete alkanetopo;
	if (forcefieldcaller)
		delete forcefieldcaller;
	if (spellcorrector)
		delete spellcorrector;
	delete ui;
}

void WrittingPanel::resizeEvent(QResizeEvent* event) {
	scribblearea->resize(ui->widget->size());

	int w = ui->tbtn_set->width(), h = ui->tbtn_set->height();
	w = h = std::min(w, h) + 10;
	int horizontalMove = (ui->tbtn_set->width() - w) / 2;
	int verticalMove = (ui->tbtn_set->height() - w) / 2;
	ui->tbtn_set->setMask(QRegion(horizontalMove, verticalMove, w, h, QRegion::Ellipse));
	ui->tbtn_set->setStyleSheet(QString(
		"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
		"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
		"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
	).arg("set"));

	ui->tbtn_exit->setMask(QRegion(horizontalMove, verticalMove, w, h, QRegion::Ellipse));
	ui->tbtn_exit->setStyleSheet(QString(
		"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
		"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
		"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
	).arg("exit"));

	ui->tbtn_undo->setMask(QRegion(horizontalMove, verticalMove, w, h, QRegion::Ellipse));
	ui->tbtn_undo->setStyleSheet(QString(
		"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
		"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
		"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
	).arg("return"));

	ui->tbtn_cls->setMask(QRegion(horizontalMove, verticalMove, w, h, QRegion::Ellipse));
	ui->tbtn_cls->setStyleSheet(QString(
		"QToolButton{ border-image:url(:/Resources/images/%1-476.png)}"
		"QToolButton:hover{border-image:url(:/Resources/images/%1-576.png)}"
		"QToolButton:pressed{border-image:url(:/Resources/images/%1-676.png)}"
	).arg("cls"));

	emit ui->tbtn_rubber->clicked(true);
}

void WrittingPanel::acceptStrokeData(list<Stroke>* _script) {
	curScript = _script;
	if (historyScripts.empty()) {
		historyScripts.push_back(curScript);
	}
	else if (historyScripts.back() != curScript) {
		historyScripts.push_back(curScript);
	}
	cout << "收到笔迹数据，笔迹计数：" << _script->size() << endl;
}

void WrittingPanel::setDetectorImg() {
	QRect fullOutline(QPoint(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
		QPoint(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()));
	std::vector<QPoint>* points;
	int avgw(0), avgh(0);
	//  cout << "curScript->size() = " << curScript->size() << endl;
	for (auto i = curScript->begin(); i != curScript->end(); i++) {
		fullOutline = extend(fullOutline, (QRect)*i);
		avgw += i->width();
		avgh += i->height();
	}
	avgw /= curScript->size();
	avgh /= curScript->size();
	K = 170.0 / (std::max(avgw, avgh));
	//  cout << "K = " << K << endl;

	//int w = scribblearea->width(), h = scribblearea->height();
	int w = fullOutline.width() * K + 500, h = fullOutline.height() * K + 500;
	w -= w % 32; h -= h % 32;//  yolo网络输入要求边长是32的倍数
	if (w > h) {
		h = w;
	}
	else {
		w = h;
	}
	if (QSize(w, h) != detectorImg.size()) {
		detectorImg = QPixmap(w, h).toImage();
	}
	detectorImg.fill(Qt::white);

	innerBias = -fullOutline.topLeft();
	outterBias.setX((fullOutline.width() * K - detectorImg.width()) / 2);
	outterBias.setY((fullOutline.width() * K - detectorImg.height()) / 2);
	outterBias = -outterBias;

	QPainter painter(&detectorImg);
	painter.setPen(QPen(Qt::black, 10));

	for (auto i = curScript->begin(); i != curScript->end(); i++) {
		points = i->getPoints();
		switch (points->size()) {
		case 0:
			break;
		case 1:
			painter.drawLine(
				outterBias + K * (points->at(0) + innerBias),
				outterBias + K * (points->at(0) + innerBias)
			);
			break;
		default:
			for (size_t j = 1; j < points->size(); j++) {
				painter.drawLine(
					outterBias + K * (points->at(j - 1) + innerBias),
					outterBias + K * (points->at(j) + innerBias)
				);
			}
			break;
		}
	}
}

void WrittingPanel::getBoxes() {
	detector->setWorkIndex(PREDICT);
	if (detector->isWorking() || detectorImg.isNull()) {
		return;
	}
	else {
		threadpool->start(detector);
		threadpool->waitForDone();//  TODO: 改成异步同步
	}
}

void WrittingPanel::syncBoxes() {
	for (auto& i : boxes) {
		i.moveTopLeft((i.topLeft() - outterBias) / K - innerBias);
		i.setWidth(i.width() / K);
		i.setHeight(i.height() / K);
	}
	sort(boxes.begin(), boxes.end(), boxCmp);

	//return;

	QPainter painter(scribblearea->getInstBuffer());
	//  QPainter painter(scribblearea->window());
	painter.setPen(QPen(Qt::red, 4));
	qDebug() << "[DebugInfo]: yolo output:";
	for (auto& i : boxes) {
		qDebug() << i << ", class :" << i.label.c_str() << ", conf: " << i.confidence;
		painter.drawRect(i);
	}
	update();
}

void WrittingPanel::preciseClassify() {
	recognizedSymbols.clear();
	int allPoint, validPoint;
	vector<vector<int>> divWay;
	bool allowin;
	vector<QPoint>* points;
	int iterCounter(0);
	for (auto& object : boxes) {
		allowin = 1;
		iterCounter = 0;
		for (auto i = curScript->begin(); i != curScript->end(); i++) {
			points = i->getPoints();
			allPoint = points->size();
			validPoint = 0;
			for (int i = 0; i < allPoint; i++)
				if (object.contains(points->at(i)))
					validPoint++;
			if (validPoint / (1.0 + allPoint) > 0.8) {//  超过xx的点落在框框里
				if (allowin)
					divWay.push_back(vector<int>());
				allowin = 0;
				divWay.back().push_back(iterCounter);
			}
			iterCounter++;
		}
	}
	classifierImgs.resize(divWay.size(), QPixmap(32, 32).toImage());
	int xmin, ymin, xmax, ymax, wmax, w, h; float k;
	for (size_t i = 0; i < divWay.size(); i++) {
		//  收集归一化需要的比例系数
		xmin = ymin = std::numeric_limits<int>::max(); xmax = ymax = std::numeric_limits<int>::min();
		for (int index : divWay.at(i)) {
			list<Stroke>::iterator iter = curScript->begin();
			for (int j = 0; j < index; j++)
				iter++;
			points = iter->getPoints();
			for (auto& point : *points) {
				xmin = xmin < point.x() ? xmin : point.x();
				ymin = ymin < point.y() ? ymin : point.y();
				xmax = xmax > point.x() ? xmax : point.x();
				ymax = ymax > point.y() ? ymax : point.y();
			}
		}
		w = xmax - xmin;
		h = ymax - ymin;
		wmax = max(w, h);
		k = 24.0 / (wmax + 0.01);
		QPoint start(1.0 * (24 - w * k) / 2 + 4, 1.0 * (24 - h * k) / 2 + 4);
		QPoint movek(xmin, ymin);
		classifierImgs.at(i).fill(Qt::white);
		QPainter painter(&classifierImgs.at(i));
		painter.setPen(QPen(Qt::black, 3));
		for (int index : divWay.at(i)) {
			list<Stroke>::iterator iter = curScript->begin();
			for (int j = 0; j < index; j++)
				iter++;
			points = iter->getPoints();
			if (points->size() < 1)
				continue;
			painter.drawPoint(start + (points->at(0) - movek) * k);
			for (size_t j = 1; j < points->size(); j++) {
				painter.drawLine(
					start + (points->at(j - 1) - movek) * k,
					start + (points->at(j) - movek) * k
				);
			}
		}
		painter.end();
	}
	classifier->setWorkIndex(PREDICT);
	threadpool->start(classifier);
	threadpool->waitForDone();	//TODO: 改为异步

	qDebug() << "[DebugInfo]: single classifier output:";
	int index;
	for (auto& prob : topks) {
		sortFloatWithSubIndex(prob);
		index = spellcorrector->getCnnIndex(subindexes[0]);
		if (index < 0) {
			continue;
		}
		recognizedSymbols.push_back(vector<string>());
		for (auto& i : SpellCorrector::similarWords.at(index)) {
			recognizedSymbols.back().push_back(i);
		}
		//  可能性第二高的也包含一下？
		if (prob[1] > 0.1) {
			index = spellcorrector->getCnnIndex(subindexes[1]);
			if (index < 0) {
				continue;
			}
			for (auto& i : SpellCorrector::similarWords.at(index)) {
				recognizedSymbols.back().push_back(i);
			}
		}

		cout << resCode2Block(subindexes[0]) << ": " << prob[0] << "\t";
		cout << resCode2Block(subindexes[1]) << ": " << prob[1] << "\t";
		cout << resCode2Block(subindexes[2]) << ": " << prob[2] << "\t" << endl;
	}
	//cout << "recognizedSymbols.size()=" << recognizedSymbols.size() << endl;
	spellcorrector->getBestResult(recognizedSymbols, chosedSymbols);

	renderChemChars();

	/*for (auto& i : chosedSymbols) {
		cout << i << " ";
	}
	cout << endl;*/

}

void WrittingPanel::renderChemChars() {
	//renderData.clear();
	renderData.append("<html><h1>");
	for (auto& word : chosedSymbols) {
		if (SpellCorrector::getWordType(word) == SpellCorrector::EXPLICTCUT) {
			renderData.append(word.c_str());
		}
		else {
			renderData.append(chembalancer->addHtml(word).c_str());
		}
	}
	renderData.append("</h1></html>");
	ui->historyPanel->setText(renderData);
	QTextCursor cursor = ui->historyPanel->textCursor();
	cursor.movePosition(QTextCursor::End);
	ui->historyPanel->setTextCursor(cursor);

}