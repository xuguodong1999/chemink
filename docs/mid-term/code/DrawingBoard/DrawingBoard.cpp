#include "DrawingBoard.h"
#include "ChemicalEquationInterpreter/Equation.h"
#include "MojoCNN/PatternRecognition.h"
DrawingBoard::DrawingBoard(QWidget *parent)
	: QWidget(parent) {
	initCNN();
	ui.setupUi(this);
	keyBoard.setParent(this); keyBoard.move(0, 100); keyBoard.resize(width(), height() - 400);
	ui.inputText->setStyleSheet("background:transparent;border-width:1;border-style:outset"); ui.outputText->setStyleSheet("background:transparent;border-width:0;border-style:outset");
	_tEdit.setFont(QFont("微软雅黑", 16, QFont::Bold)); _tEdit.hide();	_tEdit.setParent(this);
	setMouseTracking(true);
	_Rmenu.addAction("save  \tCtrl+S", this, SLOT(SavePic()));
	_Rmenu.addAction("quit  \tCtrl+C", this, SLOT(exitSlot()));
	_Rmenu.setStyleSheet("background-color:rgb(199,237,204)");
	connect(&keyBoard, SIGNAL(sendData(QString)), this, SLOT(receiveKeyBoardData(QString)));
	connect(&keyBoard, SIGNAL(sendCloseSignalforKeyBoard()), this, SLOT(closeKeyBoard()));	connect(&_tEdit, SIGNAL(textChanged()), this, SLOT(AddTexts()));
	connect(&setBoard, SIGNAL(setDrawingMode1()), this, SLOT(Lines()));						connect(&setBoard, SIGNAL(setDrawingMode2()), this, SLOT(Rects()));
	connect(&setBoard, SIGNAL(setDrawingMode3()), this, SLOT(Ellipses()));					connect(&setBoard, SIGNAL(setDrawingMode4()), this, SLOT(Line()));
	connect(&setBoard, SIGNAL(setDrawingMode5()), this, SLOT(Texts()));						connect(&setBoard, SIGNAL(setDrawingMode6()), this, SLOT(OpenPic()));
	connect(&setBoard, SIGNAL(setDrawingMode7()), this, SLOT(SavePic()));					connect(&setBoard, SIGNAL(setDrawingMode8(int)), this, SLOT(setColor(int)));
	connect(&setBoard, SIGNAL(setDrawingMode9(int)), this, SLOT(setStyle(int)));			connect(&setBoard, SIGNAL(setDrawingMode10(int)), this, SLOT(setWidth(int)));
	clear();
}
DrawingBoard::~DrawingBoard()
{
	input.clear(); output.clear(); _lines.clear(); _rects.clear(); _ellipse.clear(); _line.clear(); _text.clear();	_tpoint.clear();
	this->close();
}
void DrawingBoard::clear() {
	input = "  Input:  "; output = "  Output:  ";
	keyBoard.hide(); setBoard.hide();
	_lpress = false; _drawType = 1; _drag = 0; _openflag = 0; style = 1; color = Qt::black; 
	weight = (width() + height() - 200) / 300;
	_lines.clear(); _rects.clear(); _ellipse.clear(); _line.clear(); _text.clear();	_tpoint.clear(); _color.clear(); _style.clear(); _weight.clear(); _shape.clear(); _begin = pos();
	ui.inputText->setText(input); ui.outputText->setText(output); ui.outputText->show();
	update();
}
void DrawingBoard::exitSlot() {
	emit sendCloseSignalforDrawingBoard();
}
void DrawingBoard::openKeyBoard() {
	_drawType = -1;
	setBoard.hide(); keyBoard.show();
	ui.outputText->hide();
}
void DrawingBoard::closeKeyBoard() {
	_drawType = 1;
	keyBoard.hide();
	ui.outputText->show();
}
void DrawingBoard::receiveKeyBoardData(QString _data) {
	input = ui.inputText->toPlainText();
	if (_data == "←" && input.length() > 10)input.chop(1);
	else input += _data;
	ui.inputText->setText(input);
}
void DrawingBoard::paintEvent(QPaintEvent *) {
	if (_openflag == 0) {
		_pixmap = QPixmap(size());
		_pixmap.fill(Qt::white);
	}
	QPainter painter(&_pixmap);
	unsigned int i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
	QPen pen;
	for (int c = 0; c < _shape.size(); ++c) {
		pen.setStyle((Qt::PenStyle)_style.at(c)); pen.setWidth(_weight.at(c)); pen.setColor(_color.at(c));
		painter.setPen(pen);
		if (_shape.at(c) == 1) {
			const std::vector<QPoint>& line = _lines.at(i1++);
			int j;
			for (j = 0; j < line.size() - 1; ++j) {
				painter.drawLine(line.at(j), line.at(j + 1)); painter.drawPoint(line.at(j));
			}painter.drawPoint(line.at(j));
		}
		else if (_shape.at(c) == 2)
			painter.drawRect(_rects.at(i2++));
		else if (_shape.at(c) == 3)
			painter.drawEllipse(_ellipse.at(i3++));
		else if (_shape.at(c) == 4) {
			painter.drawLine(_line.at(i4).topLeft(), _line.at(i4).bottomRight()); i4++;
		}
		else if (_shape.at(c) == 5) {
			painter.setFont(QFont("微软雅黑", 16, QFont::Bold)); painter.drawText(_tpoint.at(i5), _text.at(i5)); i5++;
		}
	}painter.end();
	painter.begin(this); painter.drawPixmap(0, 0, _pixmap);
}
void DrawingBoard::AddTexts()
{
	QString& last = _text.back();
	last = _tEdit.toPlainText();
}
void DrawingBoard::mousePressEvent(QMouseEvent *e) {
	if (e->button() == Qt::LeftButton)
	{
		if (_drawType == 1)
		{
			_lpress = true;
			std::vector<QPoint> line;
			_lines.push_back(line);
			std::vector<QPoint>& lastLine = _lines.back();
			lastLine.push_back(e->pos());
			_shape.push_back(1);
		}
		else if (_drawType == 2)
		{
			_lpress = true;
			if (!_drag)
			{
				QRect rect;
				_rects.push_back(rect);
				QRect& lastRect = _rects.back();
				lastRect.setTopLeft(e->pos());
				_shape.push_back(2);
			}
			else if (_rects.back().contains(e->pos()))
			{
				_begin = e->pos();
			}
		}
		else if (_drawType == 3)
		{
			_lpress = true;
			if (!_drag)
			{
				QRect rect;
				_ellipse.push_back(rect);
				QRect& lastEllipse = _ellipse.back();
				lastEllipse.setTopLeft(e->pos());
				_shape.push_back(3);
			}
			else if (_ellipse.back().contains(e->pos()))
			{
				_begin = e->pos();
			}
		}
		else if (_drawType == 4)
		{
			_lpress = true;
			QRect rect;
			_line.push_back(rect);
			QRect& lastLine = _line.back();
			lastLine.setTopLeft(e->pos());
			_shape.push_back(4);
		}
		else if (_drawType == 5)
		{
			update();
			QPoint p;
			_tpoint.push_back(p);
			QPoint& lastp = _tpoint.back();
			lastp = e->pos();
			_tEdit.setGeometry(lastp.x(), lastp.y(), 400, 45);
			_tEdit.show();
			_text.push_back("");
			_tEdit.clear();
			_shape.push_back(5);
		}
		_color.push_back(color.rgb());
		_style.push_back(style);
		_weight.push_back(weight);
	}
}
void DrawingBoard::mouseMoveEvent(QMouseEvent *e) {
	if (_drag && ((_drawType == 2 && _rects.back().contains(e->pos())) || (_drawType == 3 && _ellipse.back().contains(e->pos())))) {
		setCursor(Qt::SizeAllCursor);
	}
	else {
		setCursor(Qt::ArrowCursor);
		_drag = 0;
	}
	if (_lpress) {
		if (_drawType == 1) {
			if (_lines.size() <= 0) return;
			std::vector<QPoint>& lastLine = _lines.back();
			lastLine.push_back(e->pos());
			update();
		}
		else if (_drawType == 2) {
			if (_drag == 0) {
				QRect& lastRect = _rects.back();
				lastRect.setBottomRight(e->pos());
			}
			else {
				QRect& lastRect = _rects.back();
				if (lastRect.contains(e->pos())) {
					int dx = e->pos().x() - _begin.x();
					int dy = e->pos().y() - _begin.y();
					lastRect = lastRect.adjusted(dx, dy, dx, dy);
					_begin = e->pos();
				}
			}
			update();
		}
		else if (_drawType == 3) {
			if (_drag == 0) {
				QRect& lastEllipse = _ellipse.back();
				lastEllipse.setBottomRight(e->pos());
			}
			else {
				QRect& lastEllipse = _ellipse.back();
				if (lastEllipse.contains(e->pos())) {
					int dx = e->pos().x() - _begin.x();
					int dy = e->pos().y() - _begin.y();
					lastEllipse = lastEllipse.adjusted(dx, dy, dx, dy);
					_begin = e->pos();
				}
			}
			update();
		}
		else if (_drawType == 4) {
			QRect& lastLine = _line.back();
			lastLine.setBottomRight(e->pos());
			update();
		}
	}
}
void DrawingBoard::mouseReleaseEvent(QMouseEvent *e) {
	if (_lpress) {
		if (_drawType == 1) {
			std::vector<QPoint>& lastLine = _lines.back();
			if (!lastLine.empty() && e->pos() != lastLine.back())
				lastLine.push_back(e->pos());
			_lpress = false;
		}
		else if (_drawType == 2)
		{
			QRect& lastRect = _rects.back();
			if (!_drag)
			{
				lastRect.setBottomRight(e->pos());
				this->cursor().setPos(this->cursor().pos().x() - lastRect.width() / 2, this->cursor().pos().y() - lastRect.height() / 2);
				_drag = 1;
			}
			_lpress = false;
		}
		else if (_drawType == 3)
		{
			QRect& lastEllipse = _ellipse.back();
			if (!_drag)
			{
				lastEllipse.setBottomRight(e->pos());
				this->cursor().setPos(this->cursor().pos().x() - lastEllipse.width() / 2, this->cursor().pos().y() - lastEllipse.height() / 2);
				_drag = 1;
			}
			_lpress = false;
		}
		else if (_drawType == 4)
		{
			QRect& lastLine = _line.back();
			lastLine.setBottomRight(e->pos());
			_lpress = false;
		}
		update();
	}
	func1();
}
void DrawingBoard::mouseDoubleClickEvent(QMouseEvent *) {
	ctrlZ();
	ctrlZ();
}
void DrawingBoard::contextMenuEvent(QContextMenuEvent *)
{
	_Rmenu.exec(this->cursor().pos());
}
void DrawingBoard::ctrlZ() {
	if (_shape.size() > 0) {
		switch (_shape.back()) {
		case 1: _lines.pop_back();		break;
		case 2: _rects.pop_back();		break;
		case 3: _ellipse.pop_back();	break;
		case 4: _line.pop_back();		break;
		}
		_shape.pop_back(); _color.pop_back(); _style.pop_back(); _weight.pop_back();
		_drag = 0;						update();
		func1();
	}
}
void DrawingBoard::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier)
		ctrlZ();
	else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier)
		SavePic();
	else if (e->key() == Qt::Key_C && e->modifiers() == Qt::ControlModifier)
		exitSlot();
}
void DrawingBoard::Lines()
{
	_drawType = 1;
	_tEdit.hide();
}
void DrawingBoard::Line()
{
	_drawType = 4;
	_tEdit.hide();
}
void DrawingBoard::Rects()
{
	_drawType = 2;
	_tEdit.hide();
}
void DrawingBoard::Ellipses()
{
	_drawType = 3;
	_tEdit.hide();
}
void DrawingBoard::Texts()
{
	_drawType = 5;
}
void DrawingBoard::SavePic()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("保存"), "new.jpg", "Image (*.jpg *.png *.bmp)");
	if (fileName.length() > 0) {
		_tEdit.hide();
		QPixmap pixmap(size());
		QPainter painter(&pixmap);
		painter.fillRect(QRect(0, 0, size().width(), size().height()), Qt::white);
		this->render(&painter);
		pixmap.copy(QRect(0, 0, size().width(), size().height())).save(fileName);
	}
}
void DrawingBoard::OpenPic() {
	QString picPath = QFileDialog::getOpenFileName(this, tr("打开"), "", "Image Files(*.jpg *.png)");
	if (!picPath.isEmpty()) {
		QPixmap pix;
		pix.load(picPath);
		QPainter p(&_pixmap);
		p.drawPixmap(0, 0, pix);
		_openflag = 1;
		update();
	}
}
void DrawingBoard::openSetBoard() {
	setBoard.show();
}
void DrawingBoard::closeSetBoard() {
	setBoard.hide();
}
void DrawingBoard::setStyle(int s) {
	style = s;
}
void DrawingBoard::setWidth(int w) {
	weight = w;
}
void DrawingBoard::setColor(int c) {
	color = c;
}
void DrawingBoard::resizeEvent(QResizeEvent *event) {
	QWidget::resizeEvent(event);
	int x = width();
	int y = height();
	int label_height = 0;
	if (y <= 720)label_height = 70;
	else if (y <= 1080)label_height = 120;
	else label_height = 200;
	ui.inputText->move(0, 0); ui.inputText->resize(x, 100);
	ui.outputText->move(0, y - label_height - 130 - 40); ui.outputText->resize(x, 130 + 40);
	ui.keyButton->move(0, y - label_height); ui.keyButton->resize(x / 5, label_height);
	ui.setButton->move(x*0.2, y - label_height); ui.setButton->resize(x / 5, label_height);
	ui.testButton->move(x*0.4, y - label_height); ui.testButton->resize(x / 5, label_height);
	ui.balanceButton->move(x*0.6, y - label_height); ui.balanceButton->resize(x / 5, label_height);
	ui.exitButton->move(x*0.8, y - label_height); ui.exitButton->resize(x / 5, label_height);
	keyBoard.move(0, 100); keyBoard.resize(width(), height() - 400);
}
void DrawingBoard::testSlot() {
	clear();
}
void DrawingBoard::balanceSlot() {
	output = "  Output:  ";
	Equation equa;
    std::string tmp = ui.inputText->toPlainText().replace("  Input:  ", "").toStdString();
    if (equa.exec(tmp))
		output += equa.p().c_str();
	else
		output += "Something must be wrong or maybe sth works wrong ?";
	ui.outputText->setText(output);
}
QString getRandString(int _length) {
	QString tmp = QString("0123456789qazwsxedcrfvtgbyhnujmikolpABCDEFGHIJKLMNOPQRSTUVWZYZ");
	QString str = QString();
	QTime t = QTime::currentTime();
	qsrand(t.msec() + t.second() * 1000);
	for (int i = 0; i < _length; i++)
		str[i] = tmp.at(qrand() % tmp.length());
	return str;
}
void DrawingBoard::makeFloatImg(int _i, int _j, std::vector<float>& _image, bool Print2Ctrl) {
	int extendScale = 60; int strokeWidth = 13;
	MyScript singleChar; singleChar.setScript(_lines, _i, _j);
	QPixmap pix = QPixmap(singleChar.length + extendScale, singleChar.width + extendScale); pix.fill(Qt::white);
	QPainter painter(&pix); QPen pen;  pen.setStyle(Qt::PenStyle::SolidLine); pen.setWidth(strokeWidth); pen.setColor(Qt::black); painter.setPen(pen);
	QPoint start, end;
	for (int k = _i; k <= _j; k++) {
		std::vector<QPoint>* line = &_lines.at(k);
		end = line->at(0); end.setX(end.x() - singleChar.v.x + extendScale / 2); end.setY(end.y() - singleChar.v.y + extendScale / 2);
		for (int j = 0; j < line->size() - 1; ++j) {
			start = line->at(j); end = line->at(j + 1); start.setX(start.x() - singleChar.v.x + extendScale / 2); start.setY(start.y() - singleChar.v.y + extendScale / 2); end.setX(end.x() - singleChar.v.x + extendScale / 2); end.setY(end.y() - singleChar.v.y + extendScale / 2);
			painter.drawPoint(start); painter.drawLine(start, end);
		}painter.drawPoint(end);
	}painter.end();
	QPixmap newpix = pix.scaled(28, 28); QImage img = newpix.toImage();
	for (int y = 0; y < img.height(); y++) {
		for (int x = 0; x < img.width(); x++) {
			QColor oldColor = QColor(img.pixel(x, y));
			float average = oldColor.blackF();
			if (average < 0.5) {
				if (Print2Ctrl)printf("**");
				_image.push_back(1.0);
			}
			else {
				if (Print2Ctrl)printf("  ");
				_image.push_back(-1.0);
			}
		}if (Print2Ctrl)std::cout << std::endl;
	}if (Print2Ctrl)std::cout << "_____________________________________________________________\n";
}
void DrawingBoard::func0(std::vector<std::vector<float>> &images, std::vector<int>& subMark) {
	MyScript script; 
	_location_of_prefix.clear(); _location_of_prefix.push_back(0);
	script.setScript(_lines);
	int countStroke = script.script.size();
	if (countStroke == 0)
		return;
	else if (countStroke == 1) {
		std::vector<float> image;
		subMark.push_back(-1);
		makeFloatImg(0, 0, image, 0);
		images.push_back(image);
	}
	else {
		std::vector<std::vector<int>> div;
		std::vector<int> index; index.push_back(0);
		for (int i = 1; i < countStroke; i++) {
			bool flag = 0;
			if (index.size() != 0) {
				for (int j = index.front(); j < i; j++) {
					if (R1BelongToR2(script.script.at(j).v, script.script.at(j).length, script.script.at(j).width,
						script.script.at(i).v, script.script.at(i).length, script.script.at(i).width)) {
						flag = 1; break;
					}
				}
			}
			if (flag) index.push_back(i);
			else {
				div.push_back(index); index.clear(); index.push_back(i);
			}
			if (i == countStroke - 1)div.push_back(index);
		}
		int wordLen = 0;
		for (int i = 0; i < div.size(); i++) {
			if (IsEqual(div.at(i).front(), div.at(i).back())) {
				subMark.push_back(-2); 
				 _location_of_prefix.push_back(wordLen);
			}
			else if (IsPlus(div.at(i).front(), div.at(i).back())) {
				subMark.push_back(-3); 
				 _location_of_prefix.push_back(wordLen);
			}
			else {
				subMark.push_back(-1); 
				std::vector<float> image;
				makeFloatImg(div.at(i).front(), div.at(i).back(), image, 0);
				images.push_back(image);
				wordLen++;
			}
		}
	}
}
void DrawingBoard::func1() {
	std::vector<std::vector<float>> images;
	std::vector<int> subMark;
	func0(images, subMark);
	std::string tmp1 = recognize_pattern_mode(images);
	std::string tmp2;
	int a, len = subMark.size();
	for (int i = 0; i < len; i++) {
		a = subMark.back(); subMark.pop_back();
		switch (a) {
		case -1:
			tmp2 += tmp1.back(); tmp1.pop_back(); break;
		case -2:
			tmp2 += "="; break;
		case -3:
			tmp2 += "+"; break;
		}
	}
	std::reverse(tmp2.begin(), tmp2.end());
	std::cout << "Show as \"" << tmp2 << "\";" << std::endl;
	input = QString("  Input:  ") + tmp2.c_str();
	ui.inputText->setText(input);
}
bool DrawingBoard::IsPlus(int _i, int _j) {
	int stdK = 3;
	if (_j - _i != 1) { return 0; }
	MyStroke s1, s2;
	s1.setMyStroke(_lines.at(_i)); s2.setMyStroke(_lines.at(_j));
	double k1, k2; bool Isline1, Isline2;
	Isline1 = s1.length > s1.width; Isline2 = s2.length > s2.width;
	if (Isline1^Isline2 == 0) { return 0; }
	k1 = Isline1 ? s1.length / s1.width : s1.width / s1.length;
	k2 = Isline2 ? s2.length / s2.width : s2.width / s2.length;
	if (k1 < stdK || k2 < stdK) { return 0; }
	MyPoint
		a1(_lines.at(_i).front().x(), _lines.at(_i).front().y()),
		a2(_lines.at(_i).back().x(), _lines.at(_i).back().y()),
		b1(_lines.at(_j).front().x(), _lines.at(_j).front().y()),
		b2(_lines.at(_j).back().x(), _lines.at(_j).back().y()),
		c1((a1.x + a2.x) / 2, (a1.y + a2.y) / 2),
		c2((b1.x + b2.x) / 2, (b1.y + b2.y) / 2);
    double dis1 = distance(a1, a2);//, dis2 = distance(b1, b2);
	if (distance(c1, c2) > dis1 / stdK) { return 0; }
	return true;
}
bool DrawingBoard::IsEqual(int _i, int _j) {
	int stdK = 3;
	if (_j - _i != 1) { return 0; }
	MyStroke s1, s2;
	s1.setMyStroke(_lines.at(_i)); s2.setMyStroke(_lines.at(_j));
	double k1, k2; bool Isline1, Isline2;
	Isline1 = s1.length > s1.width; Isline2 = s2.length > s2.width;
	if (Isline1^Isline2 == 1) { return 0; }
	k1 = Isline1 ? s1.length / s1.width : s1.width / s1.length;
	k2 = Isline2 ? s2.length / s2.width : s2.width / s2.length;
	if (k1 < stdK || k2 < stdK) { return 0; }
	MyPoint
		a1(_lines.at(_i).front().x(), _lines.at(_i).front().y()),
		a2(_lines.at(_i).back().x(), _lines.at(_i).back().y()),
		b1(_lines.at(_j).front().x(), _lines.at(_j).front().y()),
		b2(_lines.at(_j).back().x(), _lines.at(_j).back().y()),
		c1((a1.x + a2.x) / 2, (a1.y + a2.y) / 2),
		c2((b1.x + b2.x) / 2, (b1.y + b2.y) / 2);
    double dis1 = distance(a1, a2);//, dis2 = distance(b1, b2);
	if (fabs(c1.x - c2.x) > dis1 / stdK) { return 0; }
	return true;
}
