#pragma once
#include "stdafx.h"
#include "scribblescene.h"

class ScribbleView :public QGraphicsView {
	Q_OBJECT
public:
	ScribbleView(ScribbleScene* scene, QWidget* parent = nullptr);
	ScribbleScene* myScene;
	void setMode(ScribbleScene::SCRIBBLEMODE _mode);
signals:
	void sendPointsData(std::vector<QPoint>&);
	void sendMousePos(const QPoint&);
	void sendCloseSignal(const QString filename);
	void sendIsHideSignal(bool isHide);
public slots:
	void zoomIn();
	void zoomOut();
protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event)override;
	void wheelEvent(QWheelEvent* event)override;
	void paintEvent(QPaintEvent* event) override;
	void closeEvent(QCloseEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;
private:
	std::vector<QPoint> curPointsData;
	bool mousePressed;
	QString viewName;
	double zoomScale;
	QPoint lastPos;

	//void dragEnterEvent(QDragEnterEvent* event) override;
	//void dragLeaveEvent(QDragLeaveEvent* event) override;
	//void dragMoveEvent(QDragMoveEvent* event) override;
	//void dropEvent(QDropEvent* event) override;

};
