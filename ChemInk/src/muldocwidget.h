#pragma once
#include "stdafx.h"

class MulDocWidget :public QMdiArea {
	Q_OBJECT
public:
	MulDocWidget(QWidget* parent = nullptr);
signals:
	//void sendSize(const QSize& size);
protected:
	void resizeEvent(QResizeEvent* event)override;

};
