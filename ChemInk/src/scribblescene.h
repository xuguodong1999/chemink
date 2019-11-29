#pragma once
#include "stdafx.h"

//  表示一张单图层画布，包含各种图元
class ScribbleScene :public QGraphicsScene {
	Q_OBJECT
public:
	enum SCRIBBLEMODE {
		Mode_Pen,
		Mode_Rect, Mode_Ellipse, Mode_Polygon, Mode_Chem,
		Mode_Drag_Pel,
		Mode_Drag_Scene
	}mode;
	ScribbleScene(QWidget* parent = nullptr);
	~ScribbleScene()override;
	QPixmap instPixBuffer, fixPixBuffer;
	QPainterPath curPainterPath;
	QGraphicsPathItem* curPathItem;
	int pelindex;//  图元索引
	QMap<int, QGraphicsPathItem*> pathMap;
	QMap<int, QGraphicsEllipseItem*> ellipseMap;
	QMap<int, QGraphicsRectItem*> rectMap;
	QMap<int, QGraphicsLineItem*> lineMap;
	QMap<int, QGraphicsPixmapItem*> pixmapMap;
	QMap<int, QGraphicsPolygonItem*> polygonMap;
	//Script script;//  存储一张画布上的所有笔迹的副本
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;


	//void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
	//void dragLeaveEvent(QGraphicsSceneDragDropEvent* event) override;
	//void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
	//void dropEvent(QGraphicsSceneDragDropEvent* event) override;

};
