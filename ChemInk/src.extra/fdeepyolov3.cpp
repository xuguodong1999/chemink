#include "fdeepyolov3.h"
using namespace fdeep;
#include <QImage>
#include <QThread>
#include <vector>
#include <QDebug>
#include <ctime>
using namespace std;

FdeepYolov3::FdeepYolov3() :kerasModel(nullptr), outputDimension(0), weightsfile(nullptr), inputs(nullptr), outputs(nullptr) { ; }

FdeepYolov3::~FdeepYolov3() {

}


void FdeepYolov3::setInOut(std::vector<QImage>* _inputs, vector<vector<DetectionObject>>* _outputs) {
	inputs = _inputs;
	outputs = _outputs;
}

void FdeepYolov3::setWeightsfile(const char* w) {

}

void FdeepYolov3::setOutputDimension(const int outdim) {
	outputDimension = outdim;
}

vector<vector<DetectionObject>>* FdeepYolov3::getDetectedObjects() {
	return outputs;
}

void FdeepYolov3::load() {
	if (kerasModel)
		delete kerasModel;
	kerasModel = (model*)calloc(1, sizeof(model));
	*kerasModel = load_model(weightsfile);
}

void FdeepYolov3::predict() {
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
	time_t start = clock();
	tensor5s_vec outputTensor = kerasModel->predict_multi(inputTensor, true);
	cout << "预测用时" << clock() - start << "ms" << endl;

	cout << "outputTensor.size() = " << outputTensor.size() << endl;
	cout << "outputTensor.at(0).size() = " << outputTensor.at(0).size() << endl;
	cout << setprecision(4) << setw(5);
	for (size_t i = 0; i < outputTensor.at(0).size(); i++) {
		tensor5& current = outputTensor.at(0).at(i);
		//cout << "current.at("<<i<<").depth() = " << current.depth() << endl;
		//cout << "current.at("<<i<<").width() = " << current.width() << endl;
		//cout << "current.at(" << i << ").height() = " << current.height() << endl;
		size_t blockSize = current.depth() / 3;
		for (size_t p = 0; p < current.width(); p++) {
			for (size_t q = 0; q < current.height(); q++) {
				for (size_t j = 4; j < blockSize; j++)
					current.set(0, 0, p, q, j, 1.0 / (1.0 + exp(-current.get(0, 0, p, q, j))));
				for (size_t j = blockSize + 4; j < 2 * blockSize; j++)
					current.set(0, 0, p, q, j, 1.0 / (1.0 + exp(-current.get(0, 0, p, q, j))));
				for (size_t j = 2 * blockSize + 4; j < 3 * blockSize; j++)
					current.set(0, 0, p, q, j, 1.0 / (1.0 + exp(-current.get(0, 0, p, q, j))));
			}
		}
		//  打印最值
		float maxProb = -0x3f3f3f3f, tmp = 0;
		for (size_t p = 0; p < current.width(); p++) {
			for (size_t q = 0; q < current.height(); q++) {
				//maxProb = -0x3f3f3f3f;
				for (size_t j = 5; j < blockSize; j++) {
					tmp = current.get(0, 0, p, q, j) *current.get(0, 0, p, q, 4);
					maxProb = maxProb > tmp ? maxProb : tmp;
					//break;
				}
				//cout << maxProb << endl;
				//maxProb = -0x3f3f3f3f;
				for (size_t j = blockSize + 5; j < 2 * blockSize; j++) {
					tmp = current.get(0, 0, p, q, j) * current.get(0, 0, p, q, 4);
					maxProb = maxProb > tmp ? maxProb : tmp;
					//break;
				}
				//cout << maxProb << endl;
				//maxProb = -0x3f3f3f3f;
				for (size_t j = 2 * blockSize + 5; j < 3 * blockSize; j++) {
					tmp = current.get(0, 0, p, q, j) * current.get(0, 0, p, q, 4);
					maxProb = maxProb > tmp ? maxProb : tmp;
				}
				//cout << maxProb << endl;
			}
		
			// 打印一组x*x feature map的极值
			cout << maxProb << endl;
		}
		


	}
	//ofstream ofsm("./tmp_yolo_output.txt");
	//ofsm << fdeep::show_tensor5s(outputTensor.at(0)) << std::endl;
	//ofsm.close();
}
