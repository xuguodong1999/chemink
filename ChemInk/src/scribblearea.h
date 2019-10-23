#pragma once

#include <QWidget>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPen>

#include <vector>
#include <list>

enum DrawingStyle { BLACKBOARD, WHITEBOARD, PAPER, RICEPAPER };

class Stroke :public QRect{
	std::vector<QPoint> points;
public:
    Stroke(QPoint& _p);
    ~Stroke();
    void push_back(QPoint& _p);
	void setRect();// 在一画写完以后再调用，用时应该小于起落笔动作，不影响流畅度
	//  void contains(QPoint& _p);
    QPoint& at(int i);
	std::vector<QPoint>* getPoints();
};

//  维护书写的用户体验，维护笔迹数据，维护可点击区域
class ScribbleArea : public QWidget {
	Q_OBJECT
private:
	std::list<Stroke>* script;// 记录历史数据的时候，直接把这个指针的空间转给上层调用者记录
	std::vector<QRect> rubber;
public:
	ScribbleArea(QWidget* parent = nullptr);
	~ScribbleArea();
	std::list<Stroke>* getScript();
	QPixmap* getInstBuffer();
	void clear();
signals:
	void sendStrokeData(std::list<Stroke>* _script);
	void askForRecognize();
	
public slots:
	void acceptRubber(bool _isPen);
	void cancelStroke();//  消灭rubber碰到的所有笔画，注意list迭代器的规则
	void undo();// 撤销最近的绘图事件
	void reloadStroke(std::list<Stroke>* _script);
	void loadStyle(DrawingStyle _drawingstyle);
	

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;

	void paintEvent(QPaintEvent* event)override;
	void resizeEvent(QResizeEvent* event) override;

private:
	//  基本画板用到的数据结构
	QPen pen;
	QPoint lastPos;
	bool isDragging;
	bool isPen;
	QPixmap instBuffer,procBuffer;	//  二级书写缓冲，缓冲只保存笔迹
	QPixmap cursorPix, backPix, brushPix, rubberPix;//  鼠标、背景、涂鸦样式
	DrawingStyle drawingstyle;
	void loadStyle();
	void drawline(const QPoint& _from, const QPoint& _to, QPaintDevice& _device);

	void repaintByBezier();
	void repaintRawScript();
};

std::vector<QPoint> bezier(std::vector<QPoint> rawpoints);
