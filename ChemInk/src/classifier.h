#pragma once
#include "stdafx.h"
#include "fdeepcaller.h"
#include <QImage>
#include <vector>

class Classifier :public FdeepCaller {
	Q_OBJECT
public:
	Classifier();
	~Classifier();
	void predict(
		const std::vector<QImage>& _imgs
	);
	std::vector<std::vector<int>> indices;
	std::vector<std::vector<float>> confidences;
	void load(const char* filename, int _outdim);
private:
	int outdim;
};

