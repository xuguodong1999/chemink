#include "stdafx.h"
#include "rawscript.h"
using namespace std;
RawScript::RawScript() :panel(nullptr) {
	classifier.load(
		(QApplication::applicationDirPath() +
			"/data/densenet-chdsbw.json").toStdString().c_str(), 78);
	detector.load(
		(QApplication::applicationDirPath() +
			"/data/chardetector-yolov3-tiny.weights").toStdString().c_str(),
			(QApplication::applicationDirPath() +
				"/data/chardetector-yolov3-tiny.cfg").toStdString().c_str());
	//  opencv4.1.1的dnn网络加载还是懒加载，即在第一次运行的时候加载
	detector.predictSingle(QImage(yoloBaseSize, QImage::Format::Format_RGB888));

}

RawScript::~RawScript() {

}
void  RawScript::setPanel(const QWidget* _panel) {
	panel = const_cast<QWidget*>(_panel);
}
void RawScript::setImage(const QPixmap* _pixmap) {
	pixmap = const_cast<QPixmap*>(_pixmap);
}
void RawScript::clear() {
	curStroke = PainterPath();
	script.clear();
}
void RawScript::pushStroke() {
	script.push_back(curStroke);
}
void RawScript::popStroke() {
	if (script.isEmpty())
		return;
	script.pop_back();
}
void RawScript::popStroke(const QPainterPath& rubberPath) {
	for (int i = 0; i < script.size(); i++) {
		if (script[i].intersects(rubberPath)) {
			script.removeAt(i);
			i--;
		}
	}
}
void RawScript::popStroke(const QPoint& innerPoint, QPoint scale) {
	for (int i = 0; i < script.size(); i++) {
		if (script[i].intersects(QRect(innerPoint - scale, innerPoint + scale))) {
			script.removeAt(i);
			i--;
		}
	}
}
const QList<RawScript::PainterPath>& RawScript::getScript() {
	return script;
}

RawScript::PainterPath::PainterPath() {

}
RawScript::PainterPath::PainterPath(const QPoint& start) :QPainterPath(start) {

}

static QImage yoloImg;
static vector<QImage> fdeepImgs;
static vector<vector<string>> srcWords;

//  返回l、m、r，表示b在a的左侧、包含、在右侧
//  未完成/暂时废弃
char RawScript::diff(const QRectF& a, const QRectF& b) {
	//  TODO:完成这个函数
	qreal aw(a.width()), ah(a.height()),
		ax1(a.topLeft().x()), ay1(a.topLeft().y()),
		ax2(a.topRight().x()), ay2(a.topRight().y()),
		ax3(a.bottomLeft().x()), ay3(a.bottomLeft().y()),
		ax4(a.bottomRight().x()), ay4(a.bottomRight().y()),
		acentx(a.center().x()), acenty(a.center().y());
	qreal bw(b.width()), bh(b.height()),
		bx1(b.topLeft().x()), by1(b.topLeft().y()),
		bx2(b.topRight().x()), by2(b.topRight().y()),
		bx3(b.bottomLeft().x()), by3(b.bottomLeft().y()),
		bx4(b.bottomRight().x()), by4(b.bottomRight().y()),
		bcentx(b.center().x()), bcenty(b.center().y());
	/*我很想把这堆参数扔进xgboost里面qaq*/
	//  缩放阈值
	qreal thresh(10);
	qreal thresh_2(2 * thresh);
	QRectF bigA(ax1 - thresh, ay1 - thresh, aw + thresh_2, ah + thresh_2);
	QRectF bigB(bx1 - thresh, by1 - thresh, bw + thresh_2, bh + thresh_2);
	if (bigA.contains(b) || bigB.contains(a))
		return 'm';
	//  比例阈值
	qreal k = 0.1;
	//  换行阈值
	qreal lineWidth = panel->height() / 3;
	//  左右判断

	return 'l';
}

const std::vector<std::vector<std::string>>& RawScript::recognize() {
	results.clear();
	if (yoloImg.size() != panel->size()) {
		yoloImg = QImage(panel->size(), QImage::Format::Format_RGB888);
	}
	yoloImg.fill(Qt::white);
	QPainter painter(&yoloImg);
	painter.setPen(QPen(Qt::black, 4));
	for (auto& i : script) {
		painter.drawPath(i);
	}
	detector.predictSingle(yoloImg.scaled(yoloBaseSize, Qt::IgnoreAspectRatio));

	//  STEP1: 获取目标检测的边框
	/*这里不关心类别和概率，只处理边框*/
	QList<QRectF> objRects;
	double
		kw(1.0 * panel->width() / yoloBaseSize.width()),
		kh(1.0 * panel->height() / yoloBaseSize.height());
	for (const int i : detector.indices) {
		objRects.push_back(QRect(
			detector.boxes[i].x * kw - 4, detector.boxes[i].y * kh - 4,
			detector.boxes[i].width * kw + 8, detector.boxes[i].height * kh + 8
		));
	}
	qDebug() << "yolov3-tiny检出边框个数：" << objRects.size();

	//  STEP2: 标记没有被检测框命中的笔画，标记他们，用决策树为他们加检测框
	/*如果一个笔画有超过比例K不在任何一个检测框里，认为这个笔画属于漏检*/
	vector<PainterPath*> leakedScriptIndices;
	for (auto& i : script) {
		int counter(0);
		for (auto& j : objRects) {
			for (int k = 0; k < i.elementCount(); k++) {
				if (j.contains(i.elementAt(k))) {
					counter++;
				}
			}
			if (counter > i.elementCount()* K_leak) {
				break;
			}
		}
		if (counter <= i.elementCount() * K_leak) {
			leakedScriptIndices.emplace_back(&i);
		}
	}
	for (size_t i = 0; i < leakedScriptIndices.size(); i++) {
		QRectF tmp = leakedScriptIndices.at(i)->boundingRect();

		objRects.push_back(QRectF(tmp.x() - 4, tmp.y() - 4, tmp.width() + 8, tmp.height() + 8));
	}
	//  STEP3: 用决策树评估检测框里的笔迹，看有没有出现合并检测的问题，如果有，根据轮廓分裂检测框

	//  STEP4: 检测框最小化，消除空白像素

	//  STEP5: 将检测框中的像素写入标准画布，提交分类器
	sort(objRects.begin(), objRects.end(),
		[=](const QRectF& a, const QRectF& b) {
			int centXa(a.center().x()), centYa(a.center().y()),
				centXb(b.center().x()), centYb(b.center().y());
			if (fabs(centYa - centYb) < panel->height() / 5) {
				return centXa < centXb;
			}
			else {
				return centYa < centYb;
			}
		}
	);
	fdeepImgs.resize(objRects.size(), QImage(fdeepBaseSize, QImage::Format::Format_RGB666));
	for (auto& i : fdeepImgs) {
		i.fill(Qt::white);
	}
	if (blankImg.size() != panel->size()) {
		blankImg = QPixmap(panel->size());
	}
	for (size_t i = 0; i < fdeepImgs.size(); i++) {
		QPainter painter(&fdeepImgs[i]);
		QPainter rpainter(&blankImg);
		rpainter.setPen(QPen(Qt::black, 6));
		blankImg.fill(Qt::white);
		for (auto& j : script) {
			int counter = 0;
			for (int k = 0; k < j.elementCount(); k++) {
				if (objRects.at(i).contains(j.elementAt(k))) {
					counter++;
				}
			}
			if (counter > j.elementCount() * 0.3) {
				rpainter.drawPath(j);
			}
		}
		if (objRects.at(i).width() > objRects.at(i).height()) {
			double k = 1.0 / objRects.at(i).width() * fdeepBaseSize.width();
			painter.drawPixmap(//优先满足宽
				QRect(0, (-objRects.at(i).height() * k + fdeepBaseSize.height()) / 2,
					fdeepBaseSize.width(), objRects.at(i).height() * k), //  target rect
				blankImg,
				objRects.at(i));//  source rect
		}
		else {//优先满足长
			double k = 1.0 / objRects.at(i).height() * fdeepBaseSize.height();
			painter.drawPixmap(//优先满足宽
				QRect((-objRects.at(i).width() * k + fdeepBaseSize.width()) / 2, 0,
					objRects.at(i).width() * k, fdeepBaseSize.height()), //  target rect
				blankImg,
				objRects.at(i));//  source rect
		}
	}
	classifier.predict(fdeepImgs);

	//  STEP6: 将分类器预测到的前topk结果提交基于字典的语言修正模块
	//indices.at(i)包含了一张图片的输出向量的概率排序索引
	srcWords.resize(classifier.indices.size());
	int index;
	for (size_t i = 0; i < classifier.indices.size(); i++) {
		for (auto& j : classifier.indices.at(i)) {//  j是类别索引
			if (classifier.confidences[i][j] > K_allowIn) {
				index = corrector.getCnnIndex(j);
				if (index < 0) {
					continue;
				}
				for (auto& k : SpellCorrector::similarWords.at(index)) {
					srcWords.at(i).emplace_back(k);
				}
				srcWords.at(i).emplace_back("");
			}
			else {
				break;
			}
		}
	}
	auto it = srcWords.begin();
	while (it != srcWords.end()) {
		if (it->empty()) {
			it = srcWords.erase(it);
		}
		else {
			it++;
		}
	}
	results.resize(1);
	//qDebug() << "srcWords.size() = " << srcWords.size();
	//for (auto& i : srcWords) {
	//	cout << "***" << endl;
	//	for (auto& j : i) {
	//		qDebug() << j.c_str();
	//	}
	//	cout << "---" << endl;
	//}
	corrector.getBestResult(srcWords, results.at(0));
	results.resize(srcWords.size() + 1);
	for (size_t i = 0; i < srcWords.size(); i++) {
		for (auto& j : srcWords.at(i)) {
			results[i + 1].emplace_back(j);
		}
	}
	srcWords.clear();
	//for (auto& i : results) {
	//	cout << "***" << endl;
	//	for (auto& j : i) {
	//		qDebug() << j.c_str();
	//	}
	//}
	return results;
}
