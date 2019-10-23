#include "detectionobject.h"

DetectionObject::DetectionObject() { ; }


DetectionObject::DetectionObject(int _xmin,int _ymin,int _width,int _height){
    setX(_xmin);
    setY(_ymin);
    setWidth(_width);
    setHeight(_height);
}

bool DetectionObject::operator <(const DetectionObject& s2) const {
	return this->confidence < s2.confidence;
}

bool DetectionObject::operator >(const DetectionObject& s2) const {
	return this->confidence > s2.confidence;
}


