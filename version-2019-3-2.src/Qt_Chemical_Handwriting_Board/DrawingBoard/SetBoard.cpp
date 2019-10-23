#include "SetBoard.h"
SetBoard::SetBoard(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	ui.s9->addItem(tr("SolidLine"), static_cast<int>(Qt::SolidLine));
	ui.s9->addItem(tr("DashLine"), static_cast<int>(Qt::DashLine));
	ui.s9->addItem(tr("DotLine"), static_cast<int>(Qt::DotLine));
	ui.s9->addItem(tr("DashDotLine"), static_cast<int>(Qt::DashDotLine));
	ui.s9->addItem(tr("DashDotDotLine"), static_cast<int>(Qt::DashDotDotLine));
	ui.s10->setValue(5);
}
SetBoard::~SetBoard()
{
	this->close();
}
void SetBoard::setMode1() {
	ui.s1->setChecked(1);
	ui.s2->setChecked(0);
	ui.s3->setChecked(0);
	ui.s4->setChecked(0);
	ui.s5->setChecked(0);
	emit setDrawingMode1();
}
void SetBoard::setMode2() {
	ui.s2->setChecked(1);
	ui.s1->setChecked(0);
	ui.s3->setChecked(0);
	ui.s4->setChecked(0);
	ui.s5->setChecked(0);
	emit setDrawingMode2();
}
void SetBoard::setMode3() {
	ui.s3->setChecked(1);
	ui.s2->setChecked(0);
	ui.s1->setChecked(0);
	ui.s4->setChecked(0);
	ui.s5->setChecked(0);
	emit setDrawingMode3();
}
void SetBoard::setMode4() {
	ui.s4->setChecked(1);
	ui.s2->setChecked(0);
	ui.s3->setChecked(0);
	ui.s1->setChecked(0);
	ui.s5->setChecked(0);
	emit setDrawingMode4();
}
void SetBoard::setMode5() {
	ui.s5->setChecked(1);
	ui.s2->setChecked(0);
	ui.s3->setChecked(0);
	ui.s4->setChecked(0);
	ui.s1->setChecked(0);
	emit setDrawingMode5();
}
void SetBoard::setMode6() {
	emit setDrawingMode6();
}
void SetBoard::setMode7() {
	emit setDrawingMode7();
}
void SetBoard::setMode8() {
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::black));
	if (color.isValid())
		emit setDrawingMode8(color.rgb());
}
void SetBoard::setMode9(int _d) {
	emit setDrawingMode9(_d);
}
void SetBoard::setMode10(int _d) {
	emit setDrawingMode10(_d);
}
