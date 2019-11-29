#include "stdafx.h"
#include "muldocwidget.h"
MulDocWidget::MulDocWidget(QWidget* parent)
	:QMdiArea(parent)
{
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setViewMode(QMdiArea::TabbedView);
	setTabPosition(QTabWidget::North);
	setTabsClosable(true);
	setTabsMovable(true);
	setTabShape(QTabWidget::TabShape::Rounded);
}

void MulDocWidget::resizeEvent(QResizeEvent* event) {
	QMdiArea::resizeEvent(event);
	//emit(sendSize(size()));
}

