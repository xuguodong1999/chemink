#ifndef WRITTINGPANEL_H
#define WRITTINGPANEL_H

#include "scribblearea.h"
#include "opencvyolov3.h"
#include "fdeepcnn.h"
#include "chembalancer.h"
#include "alkanetopo.h"
#include "forcefieldcaller.h"
#include "glmolarea.h"
#include "spellcorrector.h"
#include <string>
#include <QResizeEvent>
#include <QWidget>
#include <QImage>
#include <QThreadPool>
#include <QRect>
#include <vector>
#include <list>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class WrittingPanel; }
QT_END_NAMESPACE

class WrittingPanel : public QWidget {
    Q_OBJECT

public:
    WrittingPanel(QWidget *parent = nullptr);
    ~WrittingPanel();

public slots:
	void acceptStrokeData(std::list<Stroke>* _script);
	void acceptRubber(bool _isPen);
	void cleanCurScript();

private:
    Ui::WrittingPanel *ui;
	QPixmap pencilIcon, rubberIcon;

	std::list<QRect> clickableRects;

	ScribbleArea* scribblearea;

	OpencvYolov3* detector;
	std::vector<DetectionObject> boxes;
	QImage detectorImg;
	QPoint innerBias,outterBias;
	float K;
	//  通过innerbias把左上角坐标归零，
	//  乘以比例系数K至最佳检测大小，
	//  通过outterbias移到画布中央
	void _connect();
	void setDetectorImg();
	void getBoxes();//  涉及主线程外的线程
	void syncBoxes();

	void preciseClassify();

	FdeepCNN* classifier;
	std::vector<std::vector<float>> topks;
	std::vector<QImage> classifierImgs;


	QThreadPool* threadpool;
	ChemBalancer* chembalancer;
	AlkaneTopo* alkanetopo;
    ForceFieldCaller* forcefieldcaller;
	SpellCorrector* spellcorrector;

	std::list<Stroke>* curScript;
	std::vector<std::list<Stroke>*> historyScripts;

	//  遍历外层数组，取内层数组中的一个作为结果
	//  内层数组是多个备选结果
	std::vector<std::vector<std::string>> recognizedSymbols;
	std::vector<std::string> chosedSymbols;
	
	QString renderData;
	void renderChemChars();
	
protected:
	void resizeEvent(QResizeEvent* event) override;
	
};


#endif // WRITTINGPANEL_H
