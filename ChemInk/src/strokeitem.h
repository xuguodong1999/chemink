#pragma once
#include "stdafx.h"

class Strokeitem :public QGraphicsPathItem {
public:
	Strokeitem(QGraphicsPathItem* parent = nullptr);
	Strokeitem(const QPainterPath& path, QGraphicsPathItem* parent = nullptr);
private:
public:
	QRectF boundingRect()const override;
	QPainterPath shape()const override;
};