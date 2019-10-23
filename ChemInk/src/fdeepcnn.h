#pragma once

#include "threadsupport.h"
#include <vector>
#include <QImage>
#include <fdeep/fdeep.hpp>

class FdeepCNN :public ThreadSupport{
public:
	FdeepCNN();
    ~FdeepCNN()override;
	void setWeightsfile(const char* w);
	void setOutputDimension(const int outdim);
	void setInOut(std::vector<QImage>* _inputs, std::vector<std::vector<float>>* _outputs);
private:
	std::vector<std::vector<float>>* outputs;//  指向调用者开辟的空间
	fdeep::model* kerasModel;
	char* weightsfile;
	int outputDimension;
	std::vector<QImage>* inputs;
protected:
	void load()override;
	void predict()override;
};

