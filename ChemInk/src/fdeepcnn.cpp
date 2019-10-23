#include "fdeepcnn.h"
using namespace fdeep;
#include <QImage>
#include <QThread>
#include <vector>
#include <QDebug>
#include <ctime>
using namespace std;


FdeepCNN::FdeepCNN() : kerasModel(nullptr), outputDimension(0), weightsfile(nullptr), inputs(nullptr), outputs(nullptr) { ; }

FdeepCNN::~FdeepCNN() {
	if (kerasModel) {
		delete kerasModel;
		kerasModel = nullptr;
	}
}

void FdeepCNN::load() {
	if (kerasModel)
		delete kerasModel;
	kerasModel = (model*)calloc(1, sizeof(model));
	*kerasModel = load_model(weightsfile);
}

void FdeepCNN::setOutputDimension(const int outdim) {
	outputDimension = outdim;
}

void FdeepCNN::setWeightsfile(const char* w) {
	weightsfile = (char*)w;
}

void FdeepCNN::setInOut(std::vector<QImage>* _inputs, std::vector<std::vector<float>>* _outputs) {
	inputs = _inputs; outputs = _outputs;
}

void FdeepCNN::predict() {
	uchar* buffer(nullptr);
	int w(0), h(0), pixCounter(0);
	vector<uint8_t> ubuffer;
	tensor5s_vec inputTensor;
	for (auto& input : *inputs) {
		buffer = (uchar*)input.bits();
		w = input.width();
		h = input.height();
		pixCounter = w * h;
		ubuffer.resize(3 * pixCounter);
		for (int i = 0; i < pixCounter; i++)
			ubuffer[3 * i] = ubuffer[3 * i + 1] = ubuffer[3 * i + 2] = buffer[4 * i];

		inputTensor.push_back({ tensor5_from_bytes(ubuffer.data(), w, h, 3) });
	}
	tensor5s_vec outputTensor = kerasModel->predict_multi(inputTensor, true);
	outputs->resize(inputs->size());
	for (size_t i = 0; i < inputs->size(); i++) {
		outputs->at(i).resize(outputDimension);
		for (int j = 0; j < outputDimension; j++)
			outputs->at(i)[j] = outputTensor[i][0].as_vector()->at(j);
	}
}


