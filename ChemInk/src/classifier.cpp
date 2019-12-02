#include "stdafx.h"
#include "classifier.h"
#include <QThreadPool>
using namespace fdeep;
using namespace std;

Classifier::Classifier() {

}
Classifier::~Classifier() {

}
#define DEBUG
void Classifier::predict(
	const std::vector<QImage>& _imgs
) {
	int w(0), h(0), pixCounter(0);
	tensor5s_vec inputTensor, outputTensor;
	for (auto& input : _imgs) {
#ifdef DEBUG
		//QLabel* label = new QLabel();
		//label->setPixmap(QPixmap::fromImage(input));
		//label->show();
#endif // DEBUG
		w = input.width(); h = input.height();
		if (input.format() == QImage::Format::Format_RGB666) {
			inputTensor.push_back({ tensor5_from_bytes(
				input.constBits(),
				w, h, 3) });
		}
		else {
			inputTensor.push_back({ tensor5_from_bytes(
				input.convertToFormat(QImage::Format::Format_RGB666).constBits(),
				w, h, 3) });
		}
	}
	predicts(inputTensor, outputTensor);
	confidences.resize(_imgs.size());
	for (size_t i = 0; i < confidences.size(); i++) {
		confidences[i].resize(outdim);
		for (int j = 0; j < outdim; j++) {
			confidences[i][j] = outputTensor[i][0].as_vector()->at(j);
		}
	}
	// 获取索引按照概率的排序
	indices.resize(confidences.size());
	for (size_t i = 0; i < indices.size(); i++) {
		indices[i].resize(outdim);
		for (size_t j = 0; j < outdim; j++) {
			indices[i][j] = j;
		}
		sort(indices[i].begin(), indices[i].end(),
			[=](const int& a, const int& b) {
				return confidences[i][a] > confidences[i][b];
			}
		);
	}
}
void Classifier::load(const char* filename, int _outdim) {
	outdim = _outdim;
	loadModel(filename);
}