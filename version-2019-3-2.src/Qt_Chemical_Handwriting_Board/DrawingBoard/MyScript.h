#ifndef MYSCRIPT_H
#define MYSCRIPT_H
#define QPOINT
#ifdef QPOINT
#include <QPoint>
#endif
#include <list>
#include <vector>
#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>
#define pai 3.1415926
#define EPS 0.0000001
#define big 100000000
class MyPoint {
public:
	MyPoint();
	MyPoint(QPoint& _point);
	MyPoint(double _x, double _y);
	~MyPoint();
	double x, y;
};
double distance(MyPoint& _from, MyPoint& _to);
int direction(MyPoint& _from, MyPoint& _to);
class MyStroke {
public:
	MyStroke();
	MyStroke(std::vector<QPoint>& _points);
	~MyStroke();
	void clear();
	std::list<MyPoint> stroke;
	void setMyStroke(std::vector<QPoint>& _points);
	MyPoint v;
	double length, width;
};
class MyScript {
public:
	MyScript();
	~MyScript();
	std::vector<MyStroke> script;
	void setScript(std::vector<std::vector<QPoint>>& _script);
	void setScript(std::vector<std::vector<QPoint>>& _script, int _start, int _end);
	MyPoint v;
	double length, width;
};
bool R1BelongToR2(
	MyPoint& v1, double length1, double width1,
	MyPoint& v2, double length2, double width2
);
#endif

