#pragma once
#include "threadsupport.h"
#include "detectionobject.h"
#include <fdeep/fdeep.hpp>
#include <QImage>

//  没有完成 fdeep的向量是什么序啊 sigmoid处理以后的输出向量，数值不对劲啊
class FdeepYolov3 :public ThreadSupport{
public:
	FdeepYolov3();
	~FdeepYolov3();
	void setInOut(std::vector<QImage>* _inputs, std::vector<std::vector<DetectionObject>>* _outputs);
	void setWeightsfile(const char* w);
	void setOutputDimension(const int outdim);
	std::vector<std::vector<DetectionObject>>* getDetectedObjects();
private:
	fdeep::model* kerasModel;
	char* weightsfile;
	int outputDimension;
protected:
	void load()override;
	void predict()override;
	std::vector<QImage>* inputs;
	std::vector<std::vector<DetectionObject>>* outputs;
};

