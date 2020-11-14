#ifndef DRAWINGBOARD_H
#define DRAWINGBOARD_H
#include "MyScript.h"
#include "KeyBoard.h"
#include "SetBoard.h"
#include <vector>
#include <QWidget>
#include <QTime>
#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QToolBar>
#include <QAction>
#include <QPalette>
#include <QColor>
#include <QMenu>
#include <QImage>
#include <QImageReader>
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>
#include "ui_DrawingBoard.h"
class DrawingBoard : public QWidget
{
	Q_OBJECT
public:
	DrawingBoard(QWidget *parent = Q_NULLPTR);
	~DrawingBoard();
private slots:
	void exitSlot();
	void openKeyBoard();
	void closeKeyBoard();
	void receiveKeyBoardData(QString _data);
	void testSlot();
	void balanceSlot();
signals:
	void sendCloseSignalforDrawingBoard();
private:
	Ui::DrawingBoard ui;
	QString		input, output;
	KeyBoard	keyBoard;
	SetBoard	setBoard;
	int			_lpress;
	int			_drag;
	int			_drawType;
	QMenu		_Rmenu;
	int			_openflag;
	QPixmap		_pixmap;
	QTextEdit	_tEdit;
	int			style;
	int			weight;
	QColor		color;
	std::vector<std::vector<QPoint>>	_lines;
	std::vector<QRect>					_rects;
	std::vector<QRect>					_ellipse;
	std::vector<QRect>					_line;
	std::vector<QString>				_text;
	std::vector<QPoint>					_tpoint;
	std::vector<int>					_color;
	std::vector<int>					_style;
	std::vector<int>					_weight;
	std::vector<int>					_shape;
	QPoint								_begin;
	void makeFloatImg(int _i, int _j, std::vector<float>& _image, bool Print2Ctrl);
	void ctrlZ();
	void func1();
	void func0(std::vector<std::vector<float>> &images, std::vector<int>& subMark);
	bool IsPlus(int _i, int _j);
	bool IsEqual(int _i, int _j);
public slots:
	void Lines();
	void SavePic();
	void Rects();
	void Ellipses();
	void Line();
	void OpenPic();
	void Texts();
	void AddTexts();
	void setStyle(int);
	void setWidth(int);
	void setColor(int);
	void openSetBoard();
	void closeSetBoard();
	void clear();
protected:
	void paintEvent(QPaintEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void contextMenuEvent(QContextMenuEvent *);
	void keyPressEvent(QKeyEvent *e);
	void resizeEvent(QResizeEvent *event);
};
QString getRandString(int _length);
#endif
