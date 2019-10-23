#ifndef OPENCVYOLOV3_H
#define OPENCVYOLOV3_H
#include "threadsupport.h"
#include "detectionobject.h"
#include <QImage>
#include <QRect>
#include <opencv2/dnn.hpp>


class OpencvYolov3:public ThreadSupport {
public:
    OpencvYolov3();
    ~OpencvYolov3()override;
    void setWeightsfile(const char* w,const char* cfg);
    void setOutputDimension(const int outdim);
    void setInOut(QImage* _inputs, std::vector<DetectionObject>* _outputs);
	void setThreahhold(float _conf, float _nms = 0.5);
private:
    cv::dnn::Net net;
    std::vector<DetectionObject>* outputs;//  指向调用者开辟的空间
    char* weightsfile;
    char* cfgfile;
    int outputDimension;
    QImage* inputs;
	int inpWidth;        
	int inpHeight;
	float confThreshold;
	float nmsThreshold;

    std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);
    void nonMaximumSuppression(cv::Mat& frame, std::vector<cv::Mat>& outs);
protected:
    void load()override;
    void predict()override;
};

#endif // OPENCVYOLOV3_H
