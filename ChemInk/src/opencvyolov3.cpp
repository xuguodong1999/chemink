#include "opencvyolov3.h"
//  参考代码1：https://www.learnopencv.com/deep-learning-based-object-detection-using-yolov3-with-opencv-python-c/
//  参考代码2：https://blog.csdn.net/qq_25349629/article/details/84303336
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <iostream>
#include <QPixmap>
#include <QDebug>
#include <QFile>
using namespace std;

/* cv::mat和Qt::QImage互相转化，
 * 代码出处： https://blog.csdn.net/qq_17550379/article/details/78683153
 * 改为原位解析
 */
static inline cv::Mat QImage2CvMat(QImage& inImage) {
	switch (inImage.format()) {
	case QImage::Format_ARGB32:// 8-bit, 4 channel
	case QImage::Format_ARGB32_Premultiplied: {
		cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
		return mat;
	}
	case QImage::Format_RGB32:// 8-bit, 3 channel
	case QImage::Format_RGB888: {
		if (inImage.format() == QImage::Format_RGB32) {
			inImage = inImage.convertToFormat(QImage::Format_RGB888);
		}
		inImage = inImage.rgbSwapped();
		return cv::Mat(inImage.height(), inImage.width(), CV_8UC3,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
	}
	case QImage::Format_Indexed8: {// 8-bit, 1 channel
		cv::Mat  mat(inImage.height(), inImage.width(), CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);
		return mat;
	}
	default:
		qDebug() << "CVS::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
		break;
	}
	return cv::Mat();
}

static std::vector<std::string> classes;

OpencvYolov3::OpencvYolov3() :confThreshold(0.1), nmsThreshold(0.5) {

}

OpencvYolov3::~OpencvYolov3() {

}

void OpencvYolov3::setThreahhold(float _conf, float _nms) {
	confThreshold = _conf;
	nmsThreshold = _nms;
}

void OpencvYolov3::setInOut(QImage* _inputs, std::vector<DetectionObject>* _outputs) {
	inputs = _inputs;
	outputs = _outputs;
}

void OpencvYolov3::setOutputDimension(const int outdim) {
	outputDimension = outdim;
}

void OpencvYolov3::setWeightsfile(const char* w, const char* cfg) {
	weightsfile = (char*)w;
	cfgfile = (char*)cfg;
}

void OpencvYolov3::load() {
	cout << cfgfile << "开始加载" << endl;
	net = cv::dnn::readNetFromDarknet(cfgfile, weightsfile);
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
	cout << weightsfile << "加载完成，测试..." << endl;
	*inputs = QPixmap(32, 32).toImage();
	inpWidth = inpHeight = 32;
	predict();
	cout << "测试完成" << endl;

	QFile freader(":/Resources/weights/chardetector.labels");
	freader.open(QIODevice::ReadOnly | QIODevice::Text);
	char tmp[50];
	string stmp;
	//int index = 0;
	if (freader.isOpen()) {
		freader.readLine(tmp, 50);
		while (true) {
			stmp = tmp;
			stmp.pop_back();
			//cout << "case " << index++ << ": " << "return \"" << stmp << "\";" << endl;
			//cout << index++ << "\t" << stmp << endl;
			classes.push_back(tmp);
			classes.back().pop_back();
			freader.readLine(tmp, 50);
			if (freader.atEnd())
				break;
		}
	}
	else {
		std::cout << "无法打开yolo的标签文件" << std::endl;
	}
	freader.close();


}

void OpencvYolov3::predict() {
	cv::Mat frame = QImage2CvMat(*inputs);
	cv::Mat blob;
	cout << "frame.size()=" << frame.size() << endl;
	//  这里是cv::dnn模块，我要演示编译器的bug
	inpWidth = inputs->width();
	inpHeight = inputs->height();
	//cout << "(1) inpWidth=" << inpWidth << ", inpHeight=" << inpHeight << endl;

	//  理论上，如果（1）（2）的输出相同，那么，下面这一句可以被注释掉，但是，。。。
	inpWidth = inpHeight = 416;
	//cout << "(2) inpWidth=" << inpWidth << ", inpHeight=" << inpHeight << endl;

	cv::dnn::blobFromImage(
		frame, blob, 1 / 255.0, cv::Size(inpWidth, inpHeight),
		cv::Scalar(0, 0, 0), false, true
	);
	net.setInput(blob);

	//  cv::imshow("ha?", frame);
	//  cv::waitKey(0);
	std::vector<cv::Mat> outs;
	time_t start = clock();
	net.forward(outs, getOutputsNames(net));
	std::cout << "net.forward用时" << (clock() - start) << " ms" << std::endl;
	//start=clock();
	nonMaximumSuppression(frame, outs);
	//std::cout << "非极大抑制用时"<< (clock() - start)/1000.0 << " ms" << std::endl;

	//  std::cout<<outputs->size()<<std::endl;
}


std::vector<cv::String> OpencvYolov3::getOutputsNames(const cv::dnn::Net& net) {
	static std::vector<cv::String> names;
	if (names.empty()) {
		std::vector<int> outLayers = net.getUnconnectedOutLayers();
		std::vector<cv::String> layersNames = net.getLayerNames();
		names.resize(outLayers.size());
		for (size_t i = 0; i < outLayers.size(); ++i)
			names[i] = layersNames[outLayers[i] - 1];
	}
	return names;
}


void OpencvYolov3::nonMaximumSuppression(cv::Mat& frame, std::vector<cv::Mat>& outs) {
	std::vector<int> classIds;
	std::vector<float> confidences;
	std::vector<cv::Rect> boxes;
	//  收集输出矩阵中的数据
	for (size_t i = 0; i < outs.size(); ++i) {
		float* data = (float*)outs[i].data;
		for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols) {
			cv::Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
			cv::Point classIdPoint;
			double confidence;
			cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
			if (confidence > confThreshold) {
				int centerX = (int)(data[0] * frame.cols);
				int centerY = (int)(data[1] * frame.rows);
				int width = (int)(data[2] * frame.cols);
				int height = (int)(data[3] * frame.rows);
				int left = centerX - width / 2;
				int top = centerY - height / 2;
				classIds.push_back(classIdPoint.x);
				confidences.push_back((float)confidence);
				boxes.push_back(cv::Rect(left, top, width, height));
			}
		}
	}
	//  非极大抑制
	std::vector<int> indices;
	cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
	outputs->clear();
	for (size_t i = 0; i < indices.size(); ++i) {
		int idx = indices[i];
		cv::Rect& box = boxes[idx];
		outputs->push_back(DetectionObject(box.x, box.y, box.width, box.height));
		outputs->back().confidence = confidences[idx];
		outputs->back().class_id = classIds[idx];
		outputs->back().label = classes[outputs->back().class_id];
	}
}

