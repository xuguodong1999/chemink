#include "stdafx.h"
#include "browser.h"
using namespace std;
#pragma execution_character_set("UTF-8")

LabelButton::LabelButton(QWidget* parent) :QLabel(parent) {
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	setAlignment(Qt::AlignCenter);
	setStyleSheet(QString(
		"QLabel{border:2px groove gray;border-radius:10px;padding:6px 12px;background-color: rgb(222, 252, 232)}"
		"QLabel:hover{border:4px groove gray;border-radius:10px;padding:6px 12px;background-color: rgb(222, 252, 232)}"
		"QLabel:pressed{border:5px groove gray;border-radius:10px;padding:6px 12px;background-color: rgb(222, 252, 232)}"
	));
}

void LabelButton::mousePressEvent(QMouseEvent* event) {
	emit btnClicked(this);
}
Browser::Browser(QWidget* parent) :QWidget(parent), clickedBtn(nullptr), eledata(nullptr) {
	mainLayout = new QGridLayout();
	eleBoxLayout = new QHBoxLayout();
	mainLayout->setRowStretch(0, 1);
	mainLayout->setRowStretch(1, 8);
	mainLayout->setRowStretch(2, 2);
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(1, 8);
	mainLayout->setColumnStretch(2, 1);
	leftBtn = new QToolButton(this); leftBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	leftBtn->setArrowType(Qt::LeftArrow);
	rightBtn = new QToolButton(this); rightBtn->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	rightBtn->setArrowType(Qt::RightArrow);
	molWindow = new GluMolWindow(this); molWindow->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	connect(leftBtn, &QToolButton::clicked, this,
		[=]() {
			if (clickedBtn == nullptr) {
				return;
			}
			if (smilesIter == smilesDatas[clickedBtn].begin()) {
				throwErrorBox("提示", "已经是第一个同分异构体");
				return;
			}
			smilesIter--;
			updateMolWindow(smilesIter->c_str());
		}
	);
	connect(rightBtn, &QToolButton::clicked, this,
		[=]() {
			if (clickedBtn == nullptr) {
				return;
			}
			smilesIter++;
			if (smilesIter == smilesDatas[clickedBtn].end()) {
				smilesIter--;
				throwErrorBox("提示", "已经是最后一个同分异构体");
				return;
			}
			updateMolWindow(smilesIter->c_str());
		}
	);
	screen = new QLabel(this);
	screen->setAlignment(Qt::AlignCenter);
	mainLayout->addWidget(screen, 0, 0, 1, 3);
	mainLayout->addWidget(leftBtn, 1, 0, 1, 1);
	mainLayout->addWidget(molWindow, 1, 1, 1, 1);
	mainLayout->addWidget(rightBtn, 1, 2, 1, 1);
	mainLayout->addLayout(eleBoxLayout, 2, 0, 1, 3);

	setLayout(mainLayout);
}
void Browser::handleBtnClickEvent(LabelButton* address) {
	const string& word = address->plainText.toStdString().c_str();
	if (SpellCorrector::getWordType(word) == SpellCorrector::EXPLICTCUT) {
		if (word[0] == '=') {
			string input;
			for (auto& i : eledata->at(0)) {
				input.append(i);
			}
			screen->setText("<h1>" + QString(chembalancer.balance(input).c_str()) + "</h1>");
		}
	}
	else {
		auto iter = smilesDatas.find(address);
		if (word[0] == 'C') {
			clickedBtn = address;
			int c = 0;
			for (int i = 1; i < word.size(); i++) {
				if (word[i] >= '0' && word[i] <= '9') {
					c *= 10;
					c += word[i] - '0';
				}
				else { break; }
			}
			if (c == 0)c = 1;
			cout << "c=" << c << endl;
			if (iter == smilesDatas.end()) {
				smilesDatas[address] = vector<string>();
			}
			vector<string>& ref = smilesDatas[address];
			vector<int>input = { c };
			alkanetopo.getAlkaneTopo(ref, input);
			smilesIter = ref.begin();
			updateMolWindow(smilesIter->c_str());
		}
		else {
			;//  TODO:非碳
		}
	}
}
void Browser::updatelabelText() {
	if (eledata == nullptr) {
		return;
	}
	const vector<string>& curRes = eledata->at(0);

	for (auto& i : curRes) {
		LabelButton* btn = new LabelButton(nullptr);
		btn->plainText = i.c_str();
		btn->htmlText = "<html><h1>";
		if (SpellCorrector::getWordType(i) == SpellCorrector::EXPLICTCUT) {
			btn->htmlText.append(i.c_str());
		}
		else {
			btn->htmlText.append(chembalancer.addHtml(string(i)).c_str());
		}
		btn->htmlText.append("</h1></html>");
		btn->setText(btn->htmlText);
		connect(btn, &LabelButton::btnClicked, this, &Browser::handleBtnClickEvent);
		eleBoxLayout->addWidget(btn);
		elements.push_back(QPair<QString, LabelButton*>(i.c_str(), btn));
	}
}

void Browser::setEleBox(const std::vector<std::vector<std::string>>& _elements) {
	for (auto& i : elements) {
		disconnect(i.second, &LabelButton::btnClicked, this, &Browser::handleBtnClickEvent);
		eleBoxLayout->removeWidget(i.second);
		delete i.second;
	}
	elements.clear();

	eledata = const_cast<std::vector<std::vector<std::string>>*>(&_elements);
	updatelabelText();
}

void Browser::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
}
void Browser::updateMolWindow(const char* smiles) {
	cout << "smiles=" << smiles << endl;
	string coordinate = forcefield.getCoordinate(smiles);
	forcefield.getCoordinate("C");
	if (!mol.load(coordinate.c_str())) {
		throwErrorBox("坐标加载失败", "在Browser::updateMolWindow中");
		return;
	}
	molWindow->createMol3d(mol);
	update();
}
void Browser::closeEvent(QCloseEvent* event) {
	smilesDatas.clear();
	clickedBtn = nullptr;
	eledata = nullptr;
	event->ignore();
	emit iAmClosed();
}