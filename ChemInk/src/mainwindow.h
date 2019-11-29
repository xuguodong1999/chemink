#pragma once
#include "stdafx.h"
#include "scribblescene.h"
#include "scribbleview.h"
#include "preiewscrollarea.h"
#include "docwidget.h"
#include "muldocwidget.h"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow()override;
private:
	void setupConnection();
	Ui::MainWindowClass ui;
	MulDocWidget* muldocwidget;
	QProgressBar* progressbar;
	QToolButton* cloudBtn, * notifyBtn;
	PreviewScrollArea* previewarea;
	QFileDialog* filedialog;
	ScribbleView* scribbleView;

	QMap<int, ScribbleScene*> sceneMap;

	DocWidget* curDocWidget;
	int curSceneId;
	static int sceneId;
	static bool locker;
	QString modeHint;
	void autoChangeFont();
private slots:
	void newDrawDoc(const QString& filename);
	void showNotification();
	void showCloudService();
	void showAboutMe();
	void syncStatusPos(const QPoint& _pos);

public slots:
	void initProgressBar();
	//  < -1 则显示缓冲条，比如网络连接场景下就要用缓冲条
	void setProgressBar(double percentx100);
	void killProgressBar();
	void setCurDocWidget(DocWidget* window);

protected:
	void closeEvent(QCloseEvent* event)override;
	void resizeEvent(QResizeEvent* event)override;
	void focusInEvent(QFocusEvent* event)override;
	void focusOutEvent(QFocusEvent* event)override;
};
