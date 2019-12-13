#pragma once

#include <QImage>
#include <opencv2/dnn.hpp>
#include <vector>

class ObjDetector {
public:
	ObjDetector();
	void setThreshHold(float _confThreshold, float _nmsThreshold);
	void load(const char* weights, const char* cfg);
	//  将img的预测索引存入indices，遍历indices，索引boxes、confidences、classIds获得结果
	//  单张图的预测是多线程的，但禁止多线程调用单张图的预测，多张图必须一张一张来
	void predictSingle(const QImage& img);

	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	std::vector<int> indices;
private:
	cv::dnn::Net net;
	std::vector<cv::String> outBlobNames;
	void nonMaximumSuppression(const cv::Mat& frame, const std::vector<cv::Mat>& outs);
	int outdim;
	float confThreshold;
	float nmsThreshold;
};
