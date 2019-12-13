#pragma once
//  手写识别接口
#include "rawscript.h"
//  应用接口
#include "chembalancer.h"	//  化学方程式配平、化学符号解析
//  下面的两项移动到头文件"browser.h"中
//#include "alkanetopo.h"		//  饱和烷烃非光学同分异构体的拓扑生成
//#include "forcefield.h"		//  力场工具
//  应用交互面板
#include "browser.h"

#include <QWidget>
#include <QResizeEvent>
#include <QCloseEvent>
#include <QMoveEvent>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QPixmap>
#include <QPen>

// ScribbleArea仅维护像素画板，不提供任何矢量交互功能，不提供除像素橡皮和笔之外的任何画板工具，不提供undo、redo功能
class ScribbleArea : public QWidget {
	//  ScribbleArea设计成组件的风格
	Q_OBJECT
public:
	ScribbleArea(QWidget* parent = nullptr);

signals:
	void updateInScribbleArea();
protected:
	//  处理落笔和橡皮
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	//  发送更新数据
	void mouseReleaseEvent(QMouseEvent* event) override;
	//  处理ctrl+鼠标滚轮事件
	void wheelEvent(QWheelEvent* event)override;

	//  处理缓冲更新
	void paintEvent(QPaintEvent* event)override;
	//  不允许缩放地比有效像素小
	//  支持自动放大，策略：鼠标向边缘移动，且边框扩大不会超过屏幕范围，则向鼠标移动方向扩大手写区域
	void resizeEvent(QResizeEvent* event) override;
	void moveEvent(QMoveEvent* event)override;
	void closeEvent(QCloseEvent* event)override;
private:
	//  提供动态布局管理
	int scaleK;
	QToolButton* rubberBtn, * pencilBtn, * setBtn, * clsBtn;
	QLabel* screenLabel;
	void updateLayout();

	//  提供橡皮功能
	QPen pen, rubber;
	bool isPen, justResized;
	QPainterPath rubberPath;
	void repaintRawScript();

	//  笔迹管理、提供识别接口
	RawScript rs;

	//  绘图缓冲
	QPixmap* instBuffer;

	//  提供类似onenote手写识别版的自动放大功能
	QPoint lastPos, curPos;
	QRect upBorder, downBorder, leftBorder, rightBorder;
	void autoResize();

	ChemBalancer chembalancer;

	Browser browser;

	//  简单的线程同步
	int countCall;
	bool allowIn;
};
