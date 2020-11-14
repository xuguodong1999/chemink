#ifndef SETBOARD_H
#define SETBOARD_H
#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QColorDialog>
#include <QMainWindow>
#include "ui_SetBoard.h"
class SetBoard : public QWidget
{
	Q_OBJECT
public:
	SetBoard(QWidget *parent = Q_NULLPTR);
	~SetBoard();
private:
	Ui::SetBoard ui;
public slots:
	void setMode1();
	void setMode2();
	void setMode3();
	void setMode4();
	void setMode5();
	void setMode6();
	void setMode7();
	void setMode8();
	void setMode9(int _d);
	void setMode10(int _d);
signals:
	void setDrawingMode1();
	void setDrawingMode2();
	void setDrawingMode3();
	void setDrawingMode4();
	void setDrawingMode5();
	void setDrawingMode6();
	void setDrawingMode7();
	void setDrawingMode8(int _d);
	void setDrawingMode9(int _d);
	void setDrawingMode10(int _d);
};
#endif
