#include "stdafx.h"
#include "StrokeItem.h"


Strokeitem::Strokeitem(QGraphicsPathItem* parent) :
	QGraphicsPathItem(parent) {

}
Strokeitem::Strokeitem(const QPainterPath& path, QGraphicsPathItem* parent) :
	QGraphicsPathItem(path, parent) {

}
QRectF Strokeitem::boundingRect()const {
	return QRectF();
}
QPainterPath Strokeitem::shape()const {
	return QPainterPath();
}