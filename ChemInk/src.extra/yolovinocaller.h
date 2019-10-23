#pragma once

#include "fdeepyolov3.h"
#include "detectionobject.h"
#include <QImage>
#include <vector>
#include <opencv.hpp>

inline cv::Mat QImage2CvMat(QImage & inImage);

class YoloVinoCaller:public FdeepYolov3 {
public:
	YoloVinoCaller();
	~YoloVinoCaller();
private:
	std::vector<std::string> labels;
protected:
	void load()override;
	void predict()override;
};

