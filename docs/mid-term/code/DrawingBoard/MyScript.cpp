#include "MyScript.h"
double distance(MyPoint& _from, MyPoint& _to) {
	return sqrt(pow((_from.x - _to.x), 2) + pow((_from.y - _to.y), 2));
}
int direction(MyPoint& _from, MyPoint& _to) {
	double dx = _to.x - _from.x;
	double dy = -_to.y + _from.y;
	if (fabs(dx) < EPS) {
		if (dy > 0)return 2;
		else return 6;
	}
	double slope = dy / dx;
	if (slope > 1.73205081) {
		if (dy > 0)return 2;
		else return 6;
	}
	else if (slope > 0.577350269) {
		if (dy > 0)return 1;
		else return 5;
	}
	else if (slope > -0.577350269) {
		if (dx > 0)return 0;
		else return 4;
	}
	else if (slope > -1.73205081) {
		if (dy > 0)return 3;
		else return 7;
	}
	else {
		if (dy > 0)return 2;
		else return 6;
	}
}
MyPoint::MyPoint() {
	x = 0; y = 0;
}
MyPoint::MyPoint(QPoint& _point) {
	x = _point.x(); y = _point.y();
}
MyPoint::MyPoint(double _x, double _y) {
	x = _x; y = _y;
}
MyPoint::~MyPoint() {
	;
}
MyStroke::MyStroke() {
	clear();
}
MyStroke::MyStroke(std::vector<QPoint>& _points) {
	setMyStroke(_points);
}
MyStroke::~MyStroke() {
	clear();
}
void MyStroke::clear() {
	stroke.clear();
}
void MyStroke::setMyStroke(std::vector<QPoint>& _points) {
	clear();
	int countPoint = _points.size();
	double xL = std::numeric_limits<double>::max(), xR = std::numeric_limits<double>::min(), yU = std::numeric_limits<double>::max(), yD = std::numeric_limits<double>::min();
    MyPoint lastPoint = MyPoint(_points.at(0));
	stroke.push_back(lastPoint);
	MyPoint thisPoint;
	for (int i = 1; i < countPoint; i++) {
		thisPoint = MyPoint(_points.at(i));
		xL = xL < thisPoint.x ? xL : thisPoint.x; xR = xR > thisPoint.x ? xR : thisPoint.x; yU = yU < thisPoint.y ? yU : thisPoint.y; yD = yD > thisPoint.y ? yD : thisPoint.y;
		stroke.push_back(MyPoint(thisPoint));
		lastPoint = thisPoint;
	}
	v.x = xL; v.y = yU; length = fabs(xR - xL); width = fabs(yU - yD);
	if (length > 200000)length = width = 1;
}
MyScript::MyScript() {
}
MyScript::~MyScript() {
}
void MyScript::setScript(std::vector<std::vector<QPoint>>& _script) {
	script.clear();
    int countStroke = _script.size();
	double l = std::numeric_limits<double>::min(), w = std::numeric_limits<double>::min();
	double xmin = std::numeric_limits<double>::max(), ymin = std::numeric_limits<double>::max();
	for (int i = 0; i < countStroke; i++) {
		MyStroke s; s.setMyStroke(_script.at(i));
		xmin = xmin < s.v.x ? xmin : s.v.x; ymin = ymin < s.v.y ? ymin : s.v.y;
		l = l > s.v.x + s.length - xmin ? l : s.v.x + s.length - xmin; w = w > s.v.y + s.width - ymin ? w : s.v.y + s.width - ymin;
		script.push_back(s);
	}
	length = l; width = w;
	v.x = xmin; v.y = ymin;
}
void MyScript::setScript(std::vector<std::vector<QPoint>>& _script, int _start, int _end) {
	script.clear();
    double l = std::numeric_limits<double>::min(), w = std::numeric_limits<double>::min(); double xmin = std::numeric_limits<double>::max(), ymin = std::numeric_limits<double>::max();
	for (int i = _start; i <= _end; i++) {
		MyStroke s; s.setMyStroke(_script.at(i));
		xmin = xmin < s.v.x ? xmin : s.v.x; ymin = ymin < s.v.y ? ymin : s.v.y;
		l = l > s.v.x + s.length - xmin ? l : s.v.x + s.length - xmin; w = w > s.v.y + s.width - ymin ? w : s.v.y + s.width - ymin;
		script.push_back(s);
	}
	length = l; width = w;
	v.x = xmin; v.y = ymin;
}
bool R1BelongToR2(
	MyPoint& v1, double length1, double width1,
	MyPoint& v2, double length2, double width2)
{
	int scale = 30; v1.x -= scale; v1.y -= scale; width1 += scale * 2; length1 += scale * 2;
	bool res = (v1.x + length1 > v2.x && v2.x + length2 > v1.x && v1.y + width1 > v2.y && v2.y + width2 > v1.y);
	if (!res) {
		if (length1 > length2) {
			if (v1.x < v2.x + length2 / 2 && v2.x + length2 / 2 < v1.x + length1)
				res = 1;
		}
		else {
			if (v2.x < v1.x + length1 / 2 && v1.x + length1 / 2 < v2.x + length2)
				res = 1;
		}
	}
	return res;
}
