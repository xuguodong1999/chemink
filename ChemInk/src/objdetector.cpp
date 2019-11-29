#include "stdafx.h"
#include "objdetector.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

/* mat和Qt::QImage互相转化，
 * 代码出处： https://blog.csdn.net/weixin_38621214/article/details/85311048
 */
static inline Mat qimage2mat(const QImage& image) {
	cv::Mat mat;
	switch (image.format()) {
	case QImage::Format_ARGB32:
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
		break;
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
		cv::cvtColor(mat, mat, COLOR_BGR2RGB);
		break;
	case QImage::Format_Indexed8:
		mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
		break;
	}
	return mat;
}

ObjDetector::ObjDetector()
	:confThreshold(0.3), nmsThreshold(0.5) {

}

void ObjDetector::setThreshHold(float _confThreshold, float _nmsThreshold) {
	confThreshold = _confThreshold;
	nmsThreshold = _nmsThreshold;
}


void ObjDetector::load(const char* weights, const char* cfg) {
	net = dnn::readNetFromDarknet(cfg, weights);
	net.setPreferableBackend(dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(dnn::DNN_TARGET_CPU);
	vector<int> outLayers = net.getUnconnectedOutLayers();
	vector<String> layersNames = net.getLayerNames();
	outBlobNames.resize(outLayers.size());
	for (size_t i = 0; i < outLayers.size(); ++i)
		outBlobNames[i] = layersNames[outLayers[i] - 1];
}
static Mat frame;
void ObjDetector::predictSingle(const QImage& img) {
	frame = qimage2mat(img);
	Mat blob;
	dnn::blobFromImage(frame, blob, 1 / 255.0, Size(img.width(), img.height()), Scalar(0, 0, 0), true, false);
	net.setInput(blob);
	vector<Mat> outs;
	//time_t start = clock();
	net.forward(outs, outBlobNames);
	//qDebug() << "net.forward用时" << (clock() - start) << " ms" << endl;
	nonMaximumSuppression(frame, outs);
}

void ObjDetector::nonMaximumSuppression(const Mat& frame, const vector<Mat>& outs) {
	classIds.clear();
	confidences.clear();
	boxes.clear();
	indices.clear();
	Point classIdPoint;
	double confidence;
	Mat scores;
	int centerX, centerY, width, height, left, top;
	float* data;
	//  从输出矩阵中获取阈值大于confThreshold的框框
	for (size_t i = 0; i < outs.size(); ++i) {
		data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
			scores = outs[i].row(j).colRange(5, outs[i].cols);
			minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold) {
				centerX = (int)(data[0] * frame.cols);
				centerY = (int)(data[1] * frame.rows);
				width = (int)(data[2] * frame.cols);
				height = (int)(data[3] * frame.rows);
				left = centerX - width / 2;
				top = centerY - height / 2;
				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(Rect(left, top, width, height));
			}
		}
	}
	//  做非极大抑制
	dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	for (size_t i = 0; i < indices.size(); ++i) {
		int idx = indices[i];
		Rect& box = boxes[idx];
		rectangle(frame, Point(box.x, box.y), Point(box.x + box.width, box.y + box.height), Scalar(0, 0, 255));
	}
#ifdef DEBUG
	if (indices.size() > 0) {
		imshow("frame", frame);
	}
#endif // DEBUG
}


