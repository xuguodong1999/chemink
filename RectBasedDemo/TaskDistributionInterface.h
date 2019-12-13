#ifndef TASKDISTRIBUTIONINTERFACE_H
#define TASKDISTRIBUTIONINTERFACE_H
#include "DrawingBoard/DrawingBoard.h"
#include <QtWidgets/QMainWindow>
#include "ui_TaskDistributionInterface.h"
class TaskDistributionInterface : public QMainWindow
{
	Q_OBJECT
public:
	TaskDistributionInterface(QWidget *parent = Q_NULLPTR);
private:
	Ui::TaskDistributionInterfaceClass ui;
	DrawingBoard drawingboard;
private slots:
	void openDrawingBoard();
	void closeDrawingBoard();
};
#endif
