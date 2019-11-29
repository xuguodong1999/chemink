#pragma once
#include "stdafx.h"


class DocWidget :public QMdiSubWindow {
	Q_OBJECT
public:
	DocWidget(int _id, QWidget* parent = nullptr);
	int getId()const;
signals:
	void getFocus(DocWidget*);
protected:
	void closeEvent(QCloseEvent* event)override;
	void changeEvent(QEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;
	void focusInEvent(QFocusEvent* event)override;
	void focusOutEvent(QFocusEvent* event)override;
private:
	int id;
};