#include "stdafx.h"
#include "preiewscrollarea.h"
using namespace std;
PreviewScrollArea::PreviewScrollArea(QWidget* parent)
	:QScrollArea(parent), sceneMapPointer(nullptr)
{
	//setWindowFlags(Qt::FramelessWindowHint);
	//setStyleSheet("background-color:pink");
	setWindowOpacity(0.7);
}

void PreviewScrollArea::updatePreview(const vector<int>& modifiedViewIds) {
	for (auto& i : modifiedViewIds) {
		QLabel* tmpView;
		if (previewMap.find(i) == previewMap.end()) {
			tmpView = new QLabel(this);
			previewMap.insert(i, tmpView);
		}
		else {
			tmpView = previewMap[i];
			assert(tmpView != nullptr);
		}
		QPixmap pixmap(QSize(width(), 1.0 * width() / tmpView->width() * tmpView->height()));
		QPainter painter(&pixmap);
		(*sceneMapPointer)[i]->render(&painter);
		tmpView->setPixmap(pixmap);
	}
}

void PreviewScrollArea::setSceneMapPointer(QMap<int, ScribbleScene*>* _sceneMapPointer) {
	sceneMapPointer = _sceneMapPointer;
}

void PreviewScrollArea::resizeEvent(QResizeEvent* event) {

	QScrollArea::resizeEvent(event);
}
