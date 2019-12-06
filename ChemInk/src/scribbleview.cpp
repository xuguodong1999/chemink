#include "stdafx.h"
#include "scribbleview.h"

ScribbleView::ScribbleView(ScribbleScene* scene, QWidget* parent)
	:QGraphicsView(scene, parent), myScene(scene), zoomScale(1.1)
{
	setMouseTracking(true);
	//  画布移动通过方向键和鼠标拖曳实现
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setRenderHint(QPainter::Antialiasing);
	setCacheMode(QGraphicsView::CacheBackground);
	setOptimizationFlags(QGraphicsView::DontSavePainterState);
	//setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	//setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	// 设置坐标原点，1，-1为左下，1，1为左上
	setTransform(transform().scale(1, 1));
	setMode(ScribbleScene::Mode_Pen);
	setDragMode(QGraphicsView::RubberBandDrag);
	//centerOn(0, 0);
}

void ScribbleView::zoomIn() {
	scale(zoomScale, zoomScale);
}

void ScribbleView::zoomOut() {
	scale(1.0 / zoomScale, 1.0 / zoomScale);
}
void ScribbleView::wheelEvent(QWheelEvent* event) {
	if (QApplication::keyboardModifiers() != Qt::ControlModifier)
		return;
	if (event->delta() > 0)
		zoomIn();
	else
		zoomOut();
	QPoint prevPos = event->pos();
	QPointF prevScenePos = this->mapToScene(prevPos);
	scene()->setSceneRect(this->mapToScene(this->rect()).boundingRect());
	update();
}

void ScribbleView::mousePressEvent(QMouseEvent* event) {
	//qDebug() << "View::mousePress";
	//qDebug() << event->pos();
	lastPos = event->pos();
	mousePressed = true;
	switch (myScene->mode) {
	case ScribbleScene::Mode_Pen:
		//hide();
		curPointsData.clear();
		curPointsData.emplace_back(event->pos());
		QGraphicsView::mousePressEvent(event);
		break;
	case ScribbleScene::Mode_Rect:
		break;
	case ScribbleScene::Mode_Ellipse:
		break;
	case ScribbleScene::Mode_Polygon:
		break;
	case ScribbleScene::Mode_Chem:
		break;
	case ScribbleScene::Mode_Drag_Pel:
		QGraphicsView::mousePressEvent(event);
		break;
	case ScribbleScene::Mode_Drag_Scene:
		setCursor(Qt::ClosedHandCursor);
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		QGraphicsView::mousePressEvent(event);
	default:
		break;
	}
}
void ScribbleView::mouseMoveEvent(QMouseEvent* event) {
	//qDebug() << "View:mouseMove";
	//qDebug() << event->pos();
	//  发送场景坐标
	//emit sendMousePos(mapToScene(event->pos()).toPoint());
	//  发送视场坐标，这里完全不管场景坐标，场景坐标只保证窗口不跳动
	emit sendMousePos(event->pos());
	switch (myScene->mode) {
	case ScribbleScene::Mode_Pen:
		if (mousePressed) {
			curPointsData.emplace_back(event->pos());
			QGraphicsView::mouseMoveEvent(event);
		}
		break;
	case ScribbleScene::Mode_Rect:
		break;
	case ScribbleScene::Mode_Ellipse:
		break;
	case ScribbleScene::Mode_Polygon:
		break;
	case ScribbleScene::Mode_Chem:
		break;
	case ScribbleScene::Mode_Drag_Pel:
		QGraphicsView::mouseMoveEvent(event);
		update();
		break;
	case ScribbleScene::Mode_Drag_Scene: {
		if (mousePressed) {
			/*QPointF qpf(event->pos()-lastPos);
			qDebug() << qpf;
			translate(qpf.x(), qpf.y());*/
			qDebug() << rect().center();
			centerOn(rect().topLeft() - event->pos() + lastPos);
			//centerOn()
			update();
		}
		QGraphicsView::mouseMoveEvent(event);
		break;
	}
	default:
		break;
	}
	//lastPos = event->pos();
}
void ScribbleView::keyPressEvent(QKeyEvent* event) {
	qDebug() << event->key();
	switch (event->key()) {
	case Qt::Key_Up:
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		translate(0, -2);  // 上移
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		break;
	case Qt::Key_Down:
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		translate(0, 2);  // 下移
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		break;
	case Qt::Key_Left:
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		translate(-2, 0);  // 左移
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		break;
	case Qt::Key_Right:
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		translate(2, 0);  // 右移
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		break;
	case Qt::Key_Plus:  // 放大
		zoomIn();
		break;
	case Qt::Key_Minus:  // 缩小
		zoomOut();
		break;
	case Qt::Key_Space:  // 逆时针旋转
		rotate(-5);
		break;
	case Qt::Key_Enter:  // 顺时针旋转
	case Qt::Key_Return:
		rotate(5);
		break;
	default:
		QGraphicsView::keyPressEvent(event);
	}
}
void ScribbleView::mouseReleaseEvent(QMouseEvent* event) {
	qDebug() << "View::mouseRelease";
	qDebug() << event->pos();
	mousePressed = false;
	switch (myScene->mode) {
	case ScribbleScene::Mode_Pen: {
		//show();
		QImage image(size(), QImage::Format_RGB32);
		image.fill(Qt::white);
		QPainter painter(&image);
		render(&painter);   //关键函数
		image.save("tmp.jpg");
		mousePressed = false;
		emit(curPointsData);
		QGraphicsView::mouseReleaseEvent(event);
		break;
	}
	case ScribbleScene::Mode_Rect:
		break;
	case ScribbleScene::Mode_Ellipse:
		break;
	case ScribbleScene::Mode_Polygon:
		break;
	case ScribbleScene::Mode_Chem:
		break;
	case ScribbleScene::Mode_Drag_Pel:
		QGraphicsView::mouseReleaseEvent(event);
		break;
	case ScribbleScene::Mode_Drag_Scene:
		setCursor(Qt::OpenHandCursor);
		setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
		QGraphicsView::mouseReleaseEvent(event);
		break;
	default:
		break;
	}
}
void ScribbleView::setMode(ScribbleScene::SCRIBBLEMODE _mode) {
	//qDebug() << _mode;
	myScene->mode = _mode;
}
void ScribbleView::paintEvent(QPaintEvent* event) {
	qDebug() << "View::paint";
	QGraphicsView::paintEvent(event);
}
void ScribbleView::closeEvent(QCloseEvent* event) {
	int closeBox = QMessageBox::question(
		this, "手写画图板", "你确定要退出吗"
	);
	if (closeBox == QMessageBox::Yes) {
		emit(sendCloseSignal(viewName));
		event->accept();
	}
	else {
		event->ignore();
	}
}

void ScribbleView::resizeEvent(QResizeEvent* event) {
	//setViewportMargins(10, 10, 0, 0);
	QGraphicsView::resizeEvent(event);
}

//void ScribbleView::dragEnterEvent(QDragEnterEvent* event) {
//}
//void ScribbleView::dragLeaveEvent(QDragLeaveEvent* event) {
//}
//void ScribbleView::dragMoveEvent(QDragMoveEvent* event) {
//}
//void ScribbleView::dropEvent(QDropEvent* event) {
//}