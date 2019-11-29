#pragma once
#include "stdafx.h"

//  ******************  //
//  no longer in use!
class Point3D {
public:
	Point3D();
	Point3D(float _x);
	Point3D(float _x, float _y, float _z);
	Point3D(const Point3D& _p);
	~Point3D();
	float& x();
	float& y();
	float& z();
	float operator*(const Point3D& b)const;
	Point3D operator+(const Point3D& b)const;
	Point3D operator-(const Point3D& b)const;
	void operator+=(const Point3D& b);
	void operator-=(const Point3D& b);
	static float getDistance(const Point3D& a, const Point3D& b);
	friend std::ostream& operator<<(std::ostream& out, Point3D& b);
private:
	float pos[3];
};
//  ******************  //

class Atom {
public:
	Atom(const Eigen::Vector3d& _center = Eigen::Vector3d(0, 0, 0), const std::string& _elementName = "H");
	//Atom(const Point3D& _center=Point3D(0), const std::string& _elementName="H");
	~Atom();
	Eigen::Vector3d center;
	///Point3D center;
	int atomicNumber;
	float covalentRadius;
	float* colorVec;
	std::string elementName;
	//int aId;
};

class Bond {
public:
	Bond(float _level = 1.001);
	Bond(int _fromId, int _toId, float _level = 1.001);
	~Bond();
	float level;
	//int bId;
	std::vector<int> aids;
};

class Molecule {
public:
	Molecule();
	~Molecule();
	bool load(const char* inData, const char* inType = "mol2");
	void addBond(int _fromId, int _toId, float _level = 1.001);
	void addAtom(float x, float y, float z, std::string& _elementName);
	void display();
	size_t getAtomSize()const;
	size_t getBondSize()const;
	const Atom& getAtom(size_t i)const;
	const Bond& getBond(size_t i)const;
private:
	std::vector<Atom> atoms;
	std::vector<Bond> bonds;
};
