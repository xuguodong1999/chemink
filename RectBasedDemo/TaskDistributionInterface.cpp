#include "TaskDistributionInterface.h"
TaskDistributionInterface::TaskDistributionInterface(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(&drawingboard, SIGNAL(sendCloseSignalforDrawingBoard()), this, SLOT(closeDrawingBoard()));
}
void TaskDistributionInterface::openDrawingBoard() {
	this->hide();
	drawingboard.show();
}
void TaskDistributionInterface::closeDrawingBoard() {
	//drawingboard.hide();
	//this->show();
	drawingboard.close();
	this->close();
}
