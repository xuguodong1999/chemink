#include "stdafx.h"
#include "scribblescene.h"
using namespace std;

ScribbleScene::ScribbleScene(QWidget* parent)
	:QGraphicsScene(parent), pelindex((numeric_limits<int>::min)()), mode(Mode_Pen) {
	int w((numeric_limits<int>::max)());
	setSceneRect(QRect(-w / 2, -w / 2, w, w));
	QGraphicsRectItem* tmprect = new QGraphicsRectItem(0, 0, 100, 100);
	tmprect->setFlag(QGraphicsItem::ItemIsMovable, true);
	tmprect->setFlag(QGraphicsItem::ItemIsSelectable, true);
	tmprect->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	//addItem(tmprect);
	//qDebug() << "after add item:" << tmprect->scene();
	tmprect->setRotation(12);
	tmprect = new QGraphicsRectItem(50, 10, 70, 70);
	//qDebug() << "new:" << tmprect->scene();
	tmprect->setFlag(QGraphicsItem::ItemIsMovable, true);
	tmprect->setFlag(QGraphicsItem::ItemIsSelectable, true);
	tmprect->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	//addItem(tmprect);
	//qDebug() << "after add item:" << tmprect->scene();
}

ScribbleScene::~ScribbleScene() {

}

void ScribbleScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	//qDebug() << "ScribbleScene::mousePressEvent";
	//qDebug() << event->scenePos();
	switch (mode) {
	case ScribbleScene::Mode_Pen:
		curPainterPath = QPainterPath(event->scenePos());
		curPathItem = addPath(curPainterPath);
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
		QGraphicsScene::mousePressEvent(event);
		break;
	default:
		break;
	}
}

void ScribbleScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	//qDebug() << "ScribbleScene::mouseMoveEvent";
	//qDebug() << event->scenePos();
	switch (mode) {
	case ScribbleScene::Mode_Pen:
		curPainterPath.lineTo(event->scenePos());
		for (auto& i : items()) {
			if (i == curPathItem) {
				removeItem(i);
				curPathItem = addPath(curPainterPath);
				break;
			}
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
		QGraphicsScene::mouseMoveEvent(event);
		break;
	default:
		break;
	}
}

void ScribbleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	//qDebug() << "ScribbleScene::mouseReleaseEvent";
	//qDebug() << event->scenePos();
	switch (mode) {
	case ScribbleScene::Mode_Pen:
		curPathItem->setFlag(QGraphicsItem::ItemIsMovable, true);
		curPathItem->setAcceptHoverEvents(true);
		pathMap.insert(pelindex++, curPathItem);
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
		QGraphicsScene::mouseReleaseEvent(event);
		break;
	default:
		break;
	}
}


//void ScribbleScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
//
//	qDebug() << "ScribbleScene::dragEnterEvent";
//	QGraphicsScene::dragEnterEvent(event);
//}
//
//void ScribbleScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event) {
//
//	qDebug() << "ScribbleScene::dragLeaveEvent";
//	QGraphicsScene::dragLeaveEvent(event);
//}
//
//void ScribbleScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
//
//	qDebug() << "ScribbleScene::dragMoveEvent";
//	QGraphicsScene::dragMoveEvent(event);
//}
//
//void ScribbleScene::dropEvent(QGraphicsSceneDragDropEvent* event) {
//
//	qDebug() << "ScribbleScene::dropEvent";
//	QGraphicsScene::dropEvent(event);
//}
