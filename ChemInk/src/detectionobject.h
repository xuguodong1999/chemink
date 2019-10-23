#pragma once
#include <QPoint>
#include <QRect>
#include <string>

class DetectionObject :public QRect{
public:
    int class_id;
	float confidence;
	std::string label;
	DetectionObject();
    DetectionObject(int _xmin,int _ymin,int _width,int _height);

	bool operator <(const DetectionObject& s2) const;
    bool operator >(const DetectionObject& s2) const;
};

