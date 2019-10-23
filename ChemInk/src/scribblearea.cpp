#include "scribblearea.h"
#include <QDebug>
#include <QPainter>
#include <ctime>
#include <cmath>
#include <vector>
#include <algorithm>
#include <list>
#include <cstdlib>
#include <string>
#include <iostream>
using namespace std;

Stroke::Stroke(QPoint& _p) {
	points.clear();
	points.push_back(_p);
}

Stroke::~Stroke() {
	points.clear();
}

void Stroke::push_back(QPoint& _p) {
	points.push_back(_p);
}

void Stroke::setRect() {
	int xmin, ymin, xmax, ymax;
	xmin = ymin = std::numeric_limits<int>::max();
	xmax = ymax = std::numeric_limits<int>::min();
	for (auto& i : points) {
		xmin = xmin < i.x() ? xmin : i.x();
		ymin = ymin < i.y() ? ymin : i.y();
		xmax = xmax > i.x() ? xmax : i.x();
		ymax = ymax > i.y() ? ymax : i.y();
	}
	int rad = 5;
	setX(xmin - rad);
	setY(ymin - rad);
	setWidth(xmax - xmin + 2 * rad);
	setHeight(ymax - ymin + 2 * rad);
}

QPoint& Stroke::at(int i) {
	return points.at(i);
}

vector<QPoint>* Stroke::getPoints() {
	return &points;
}

ScribbleArea::ScribbleArea(QWidget* parent) :
	isDragging(false),
	isPen(false) {
	setParent(parent);
	setWindowFlags(Qt::FramelessWindowHint);

	script = new list<Stroke>;
	emit sendStrokeData(script);

	instBuffer = QPixmap(size());
	rubberPix.load(":/Resources/images/blackboard-eraser.png");
	cursorPix.load(":/Resources/images/blackboard-cursor.png");
}

list<Stroke>* ScribbleArea::getScript() {
	return script;
}

void ScribbleArea::acceptRubber(bool _isPen) {
	isPen = _isPen;
	if (isPen) {
		setCursor(QCursor(cursorPix.scaled(40, 50), 0, 0));
	}
	else {
		setCursor(QCursor(rubberPix.scaled(80, 80), 15, 30));
	}
}

ScribbleArea::~ScribbleArea() {
	/*if (script) {
		delete script;
		script = nullptr;
	}*/
	//  由组件调用者回收内存@writtingpanel
}

void ScribbleArea::loadStyle(DrawingStyle _drawingstyle) {
	drawingstyle = _drawingstyle;
	loadStyle();
}

void ScribbleArea::loadStyle() {
	switch (drawingstyle) {
	case BLACKBOARD:
		setCursor(QCursor(cursorPix.scaled(40, 50), 0, 0));
		//setCursor(QCursor(Qt::CursorShape::CrossCursor));
		brushPix.load(":/Resources/images/blackboard-brush.png");
		pen.setBrush(QBrush(brushPix));

		backPix.load(":/Resources/images/blackboard-background.png");
		break;
	case WHITEBOARD:
		setCursor(QCursor(Qt::CursorShape::CrossCursor));

		brushPix = QPixmap(100, 100); brushPix.fill(Qt::black);
		pen.setBrush(QBrush(brushPix));

		backPix = QPixmap(100, 100); backPix.fill(Qt::white);
		break;
	case PAPER:
	L:
		break;
	default:
		goto L;
	}
	pen.setCapStyle(Qt::RoundCap);
	pen.setJoinStyle(Qt::RoundJoin);
	pen.setWidth(6);
}

void ScribbleArea::drawline(const QPoint& _from, const QPoint& _to, QPaintDevice& _device) {
	QPainter painter(&_device);
	painter.setPen(pen);
	if (_from != _to) {
		painter.drawLine(_from, _to);
	}
	else {
		painter.drawPoint(_from);
	}
	int rad = (pen.width() / 2) + 2;//  局部更新
	update(QRect(_from, _to).normalized().adjusted(-rad, -rad, +rad, +rad));
}

void ScribbleArea::clear() {
	if (!script || script->empty())
		return;
	//  旧内存位于@writtingpanel的historyScripts中
	script = new list<Stroke>;
	emit sendStrokeData(script);
	repaintRawScript();
	update();
}

void ScribbleArea::cancelStroke() {
	if (!script || rubber.empty())
		return;
	bool isTarget;
	pen.setBrush(QBrush(backPix));//  笔刷置为背景色用于覆盖
	std::vector<QPoint>* points;
	//  cout << "rubber.size() = " << rubber.size() << endl;
	for (auto& i : rubber) {
		isTarget = false;
		for (auto j = script->begin(); j != script->end();) {
			for (auto& k : *(j->getPoints())) {
				if (i.contains(k)) {
					isTarget = true;
					break;
				}
			}
			if (isTarget) {
				points = j->getPoints();
				switch (points->size()) {
				case 0:
					break;
				case 1:
					drawline(points->at(0), points->at(0), instBuffer);
					break;
				default:
					for (size_t jj = 1; jj < points->size(); jj++) {
						drawline(points->at(jj - 1), points->at(jj), instBuffer);
					}
					break;
				}
				j = script->erase(j);
				break;
			}
			else {
				j++;
			}
		}
	}
	pen.setBrush(QBrush(brushPix));//  恢复笔刷
}

//  不到迫不得已，不要全局重绘
void ScribbleArea::repaintRawScript() {
	if (!script)
		return;
	instBuffer = QPixmap(size());
	//  procBuffer = QPixmap(size());
	QPainter painter(&instBuffer);
	painter.drawPixmap(QRect(x(), y(), width(), height()), backPix, backPix.rect());
	//  TODO: 完成笔迹的迁移重绘
	painter.end();
	std::vector<QPoint>* points;
	for (auto i = script->begin(); i != script->end(); i++) {
		points = i->getPoints();
		switch (points->size()) {
		case 0:
			break;
		case 1:
			drawline(points->at(0), points->at(0), instBuffer);
			break;
		default:
			for (size_t j = 1; j < points->size(); j++) {
				drawline(points->at(j - 1), points->at(j), instBuffer);
			}
			break;
		}
	}
}

void ScribbleArea::repaintByBezier() {
	if (!script)
		return;
	vector<QPoint>* pointer(nullptr);
	vector<QPoint> newScript;
	for (auto i = script->begin(); i != script->end(); i++) {
		pointer = i->getPoints();
		newScript = bezier(*pointer);
		switch (newScript.size()) {
		case 0:
			break;
		case 1:
			drawline(newScript[0], newScript[0], procBuffer);
			break;
		default:
			for (size_t j = 1; j < newScript.size(); j++) {
				drawline(newScript.at(j - 1), newScript.at(j), procBuffer);
			}
		}
	}
}

void ScribbleArea::reloadStroke(std::list<Stroke>* _script) {
	if (!script) {
		delete script;
	}
	script = _script;
}

void ScribbleArea::mousePressEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		qDebug() << "mouse press...";
		if (isPen) {
			lastPos = event->pos();
			//  drawline(lastPos, lastPos, instBuffer);
			script->push_back(Stroke(lastPos));
			isDragging = true;
		}
		else {
			rubber.clear();
			int rad = 10;
			QPoint p(rad, rad);
			rubber.push_back(QRect(event->pos() - p, event->pos() + p));
			cancelStroke();
		}
	}
	else {
		;
	}
}

void ScribbleArea::mouseMoveEvent(QMouseEvent* event) {
	//if (!isDragging)
	//	return;
	if (event->buttons() == Qt::LeftButton) {
		if (isPen) {
			drawline(lastPos, event->pos(), instBuffer);
			lastPos = event->pos();
			script->back().push_back(lastPos);
		}
		else {
			if ((rand() % 100) % 3 != 0)
				return;
			int rad = 10;
			QPoint p(rad, rad);
			rubber.push_back(QRect(event->pos() - p, event->pos() + p));
			cancelStroke();
		}
	}
	else {
		;
	}
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		qDebug() << "mouse release...";
		if (isPen) {
			drawline(lastPos, event->pos(), instBuffer);
			lastPos = event->pos();
			script->back().push_back(lastPos);
			script->back().setRect();
			isDragging = false;
			emit askForRecognize();
		}
		else {
			int rad = 10;
			QPoint p(rad, rad);
			rubber.push_back(QRect(event->pos() - p, event->pos() + p));
			cancelStroke();
		}
	
	}
	else {

	}
}

void ScribbleArea::undo() {
	if (!script || script->size() < 1)
		return;
	pen.setBrush(QBrush(backPix));//  笔刷置为背景色用于覆盖
	std::vector<QPoint>* points = script->back().getPoints();
	switch (points->size()) {
	case 0:
		break;
	case 1:
		drawline(points->at(0), points->at(0), instBuffer);
		break;
	default:
		for (size_t jj = 1; jj < points->size(); jj++) {
			drawline(points->at(jj - 1), points->at(jj), instBuffer);
		}
		break;
	}
	script->pop_back();
	pen.setBrush(QBrush(brushPix));//  恢复笔刷
}

void ScribbleArea::paintEvent(QPaintEvent* event) {
	//  qDebug() << "paint...";
	QRect rect = event->rect();
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	//  目前没有用贝塞尔曲线重绘，用不到procBuffer
	if (isDragging) {
		painter.drawPixmap(rect, instBuffer, rect);
		//painter.drawPixmap(rect, procBuffer, rect);
	}
	else {
		painter.drawPixmap(rect, instBuffer, rect);
	}

}

void ScribbleArea::resizeEvent(QResizeEvent* event) {
	repaintRawScript();
	//  instBuffer = procBuffer;
}

QPixmap* ScribbleArea::getInstBuffer() {
	//procBuffer.fill(Qt::transparent);
	//return &procBuffer;
	return &instBuffer;
}

vector<QPoint> bezier(vector<QPoint> rawpoints) {
	if (rawpoints.size() < 1)
		return rawpoints;
	const float step = 0.01;
	vector<QPoint> res;
	if (rawpoints.size() == 1) {//递归结束
		for (float t = 0; t < 1; t += step)
			res.push_back(rawpoints[0]);
		return res;
	}
	vector<QPoint> rawpoints1;
	vector<QPoint> rawpoints2;
	rawpoints1.assign(rawpoints.begin(), rawpoints.end() - 1);
	rawpoints2.assign(rawpoints.begin() + 1, rawpoints.end());


	vector<QPoint> pln1 = bezier(rawpoints1);
	vector<QPoint> pln2 = bezier(rawpoints2);
	// 已经划分成最小单元
	for (float t = 0; t < 1; t += step) {
		res.push_back(
			QPoint(
			(1.0 - t) * pln1[round(1.0 / step * t)] +
				t * pln2[round(1.0 / step * t)]
			)
		);
	}
	return res;
}
