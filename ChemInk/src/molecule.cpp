#include "stdafx.h"
#include "molecule.h"
#include <map>
#include <QString>
using namespace std;
using namespace Eigen;

static vector<vector<float>> colorTable = {	// R G B A
	{1.00000, 1.00000, 1.00000, 1.00000},	// White 
	{1.00000, 0.75294, 0.79608, 1.00000},	// Pink 
	{0.85490, 0.64706, 0.12549, 1.00000},	// Golden Rod
	{0.56078, 0.56078, 1.00000, 1.00000},	// Sky Blue 
	{0.94118, 0.00000, 0.00000, 1.00000},	// Red 
	{1.00000, 0.78431, 0.00000, 1.00000},	// Yellow 
	{0.62745, 0.12549, 0.94118, 1.00000},	// Purple
	{1.00000, 0.64706, 0.00000, 1.00000},	// Orange 
	{0.00000, 0.00000, 1.00000, 1.00000},	// Blue 
	{0.50196, 0.50196, 0.56471, 1.00000},	// Dark Grey 
	{0.64706, 0.16471, 0.16471, 1.00000},	// Brown 
	{1.00000, 0.07843, 0.57647, 1.00000},	// Deep Pink 
	{0.00000, 1.00000, 0.00000, 1.00000},	// Green 
	{0.69804, 0.13333, 0.13333, 1.00000},	// Fire Brick 
	{0.13333, 0.54510, 0.13333, 1.00000},	// Forest Green 
	{0.78431, 0.78431, 0.78431, 1.00000}	// Light Grey
};
const map<string, pair<int, float>> elementTable = {
		pair<string,pair<int,float>>("H",pair<int,float>(1,32)),
		pair<string,pair<int,float>>("He",pair<int,float>(2,93)),
		pair<string,pair<int,float>>("Li",pair<int,float>(3,123)),
		pair<string,pair<int,float>>("Be",pair<int,float>(4,90)),
		pair<string,pair<int,float>>("B",pair<int,float>(5,82)),
		pair<string,pair<int,float>>("C",pair<int,float>(6,77)),
		pair<string,pair<int,float>>("N",pair<int,float>(7,75)),
		pair<string,pair<int,float>>("O",pair<int,float>(8,73)),
		pair<string,pair<int,float>>("F",pair<int,float>(9,72)),
		pair<string,pair<int,float>>("Ne",pair<int,float>(10,71)),
		pair<string,pair<int,float>>("Na",pair<int,float>(11,154)),
		pair<string,pair<int,float>>("Mg",pair<int,float>(12,136)),
		pair<string,pair<int,float>>("Al",pair<int,float>(13,118)),
		pair<string,pair<int,float>>("Si",pair<int,float>(14,111)),
		pair<string,pair<int,float>>("P",pair<int,float>(15,106)),
		pair<string,pair<int,float>>("S",pair<int,float>(16,102)),
		pair<string,pair<int,float>>("Cl",pair<int,float>(17,99)),
		pair<string,pair<int,float>>("Ar",pair<int,float>(18,98)),
		pair<string,pair<int,float>>("K",pair<int,float>(19,203)),
		pair<string,pair<int,float>>("Ca",pair<int,float>(20,174)),
		pair<string,pair<int,float>>("Sc",pair<int,float>(21,144)),
		pair<string,pair<int,float>>("Ti",pair<int,float>(22,132)),
		pair<string,pair<int,float>>("V",pair<int,float>(23,122)),
		pair<string,pair<int,float>>("Cr",pair<int,float>(24,118)),
		pair<string,pair<int,float>>("Mn",pair<int,float>(25,117)),
		pair<string,pair<int,float>>("Fe",pair<int,float>(26,117)),
		pair<string,pair<int,float>>("Co",pair<int,float>(27,116)),
		pair<string,pair<int,float>>("Ni",pair<int,float>(28,115)),
		pair<string,pair<int,float>>("Cu",pair<int,float>(29,117)),
		pair<string,pair<int,float>>("Zn",pair<int,float>(30,125)),
		pair<string,pair<int,float>>("Ga",pair<int,float>(31,126)),
		pair<string,pair<int,float>>("Ge",pair<int,float>(32,122)),
		pair<string,pair<int,float>>("As",pair<int,float>(33,120)),
		pair<string,pair<int,float>>("Se",pair<int,float>(34,116)),
		pair<string,pair<int,float>>("Br",pair<int,float>(35,114)),
		pair<string,pair<int,float>>("Kr",pair<int,float>(36,112)),
		pair<string,pair<int,float>>("Rb",pair<int,float>(37,216)),
		pair<string,pair<int,float>>("Sr",pair<int,float>(38,191)),
		pair<string,pair<int,float>>("Y",pair<int,float>(39,162)),
		pair<string,pair<int,float>>("Zr",pair<int,float>(40,145)),
		pair<string,pair<int,float>>("Nb",pair<int,float>(41,134)),
		pair<string,pair<int,float>>("Mo",pair<int,float>(42,130)),
		pair<string,pair<int,float>>("Tc",pair<int,float>(43,127)),
		pair<string,pair<int,float>>("Ru",pair<int,float>(44,125)),
		pair<string,pair<int,float>>("Rh",pair<int,float>(45,125)),
		pair<string,pair<int,float>>("Pd",pair<int,float>(46,128)),
		pair<string,pair<int,float>>("Ag",pair<int,float>(47,134)),
		pair<string,pair<int,float>>("Cd",pair<int,float>(48,148)),
		pair<string,pair<int,float>>("In",pair<int,float>(49,144)),
		pair<string,pair<int,float>>("Sn",pair<int,float>(50,141)),
		pair<string,pair<int,float>>("Sb",pair<int,float>(51,140)),
		pair<string,pair<int,float>>("Te",pair<int,float>(52,136)),
		pair<string,pair<int,float>>("I",pair<int,float>(53,133)),
		pair<string,pair<int,float>>("Xe",pair<int,float>(54,131)),
		pair<string,pair<int,float>>("Cs",pair<int,float>(55,235)),
		pair<string,pair<int,float>>("Ba",pair<int,float>(56,198)),
		pair<string,pair<int,float>>("La",pair<int,float>(57,169)),
		pair<string,pair<int,float>>("Ce",pair<int,float>(58,165)),
		pair<string,pair<int,float>>("Pr",pair<int,float>(59,165)),
		pair<string,pair<int,float>>("Nd",pair<int,float>(60,164)),
		pair<string,pair<int,float>>("Pm",pair<int,float>(61,163)),
		pair<string,pair<int,float>>("Sm",pair<int,float>(62,162)),
		pair<string,pair<int,float>>("Eu",pair<int,float>(63,185)),
		pair<string,pair<int,float>>("Gd",pair<int,float>(64,161)),
		pair<string,pair<int,float>>("Tb",pair<int,float>(65,159)),
		pair<string,pair<int,float>>("Dy",pair<int,float>(66,159)),
		pair<string,pair<int,float>>("Ho",pair<int,float>(67,158)),
		pair<string,pair<int,float>>("Er",pair<int,float>(68,157)),
		pair<string,pair<int,float>>("Tm",pair<int,float>(69,156)),
		pair<string,pair<int,float>>("Yb",pair<int,float>(70,174)),
		pair<string,pair<int,float>>("Lu",pair<int,float>(71,156)),
		pair<string,pair<int,float>>("Hf",pair<int,float>(72,144)),
		pair<string,pair<int,float>>("Ta",pair<int,float>(73,134)),
		pair<string,pair<int,float>>("W",pair<int,float>(74,130)),
		pair<string,pair<int,float>>("Re",pair<int,float>(75,128)),
		pair<string,pair<int,float>>("Os",pair<int,float>(76,126)),
		pair<string,pair<int,float>>("Ir",pair<int,float>(77,127)),
		pair<string,pair<int,float>>("Pt",pair<int,float>(78,130)),
		pair<string,pair<int,float>>("Au",pair<int,float>(79,134)),
		pair<string,pair<int,float>>("Hg",pair<int,float>(80,149)),
		pair<string,pair<int,float>>("Tl",pair<int,float>(81,148)),
		pair<string,pair<int,float>>("Pb",pair<int,float>(82,147)),
		pair<string,pair<int,float>>("Bi",pair<int,float>(83,146)),
		pair<string,pair<int,float>>("Po",pair<int,float>(84,146)),
		pair<string,pair<int,float>>("At",pair<int,float>(85,145)),
		pair<string,pair<int,float>>("Th",pair<int,float>(90,165)),
		pair<string,pair<int,float>>("U",pair<int,float>(92,142))
};

Point3D::Point3D() {

}
Point3D::Point3D(float _x) {
	pos[0] = pos[1] = pos[2] = _x;
}
Point3D::Point3D(float _x, float _y, float _z) {
	pos[0] = _x; pos[1] = _y; pos[2] = _z;
}
Point3D::Point3D(const Point3D& _p) {
	pos[0] = _p.pos[0]; pos[1] = _p.pos[1]; pos[2] = _p.pos[2];
}
Point3D::~Point3D() {

}
float& Point3D::x() {
	return pos[0];
}
float& Point3D::y() {
	return pos[1];
}
float& Point3D::z() {
	return pos[2];
}
Point3D Point3D::operator+(const Point3D& b)const {
	return Point3D(pos[0] + b.pos[0], pos[1] + b.pos[1], pos[2] + b.pos[2]);
}
float Point3D::operator*(const Point3D& b)const {
	return pos[0] * b.pos[0] + pos[1] * b.pos[1] + pos[2] * b.pos[2];
}
Point3D Point3D::operator-(const Point3D& b)const {
	return Point3D(pos[0] - b.pos[0], pos[1] - b.pos[1], pos[2] - b.pos[2]);
}
void Point3D::operator+=(const Point3D& b) {
	pos[0] += b.pos[0]; pos[1] += b.pos[1]; pos[2] += b.pos[2];
}
void Point3D::operator-=(const Point3D& b) {
	pos[0] -= b.pos[0]; pos[1] -= b.pos[1]; pos[2] -= b.pos[2];
}
float Point3D::getDistance(const Point3D& a, const Point3D& b) {
	return sqrtf(pow(a.pos[0] - b.pos[0], 2) + pow(a.pos[1] - b.pos[1], 2) + pow(a.pos[2] - b.pos[2], 2));
}
ostream& operator<<(ostream& out, Point3D& b) {
	out << "(" << b.x() << "," << b.y() << "," << b.z() << ")";
	return out;
}

Bond::Bond(float _level) :level(_level) {
	aids.resize(2, -1);
}
Bond::Bond(int _fromId, int _toId, float _level) : level(_level) {
	aids.resize(2);
	aids.at(0) = _fromId;
	aids.at(1) = _toId;
}
Bond::~Bond() {

}

//Atom::Atom(const Point3D& _center, const string& _elementName)
//	: center(_center), elementName(_elementName), colorVec(nullptr) {
//	auto iter = elementTable.find(elementName);
//	if (iter == elementTable.end()) {
//		exit(throwErrorBox(
//			"Error element name: ",
//			elementName.c_str()
//		));
//	}
//	atomicNumber = iter->second.first;
//	covalentRadius = iter->second.second;
//	colorVec = colorTable.at(atomicNumber % colorTable.size()).data();
//}
Atom::Atom(const Eigen::Vector3d& _center, const string& _elementName)
	: center(_center), elementName(_elementName), colorVec(nullptr) {
	auto iter = elementTable.find(elementName);
	if (iter == elementTable.end()) {
		exit(throwErrorBox(
			"Error element name: ",
			elementName.c_str()
		));
	}
	atomicNumber = iter->second.first;
	covalentRadius = iter->second.second;
	colorVec = colorTable.at(atomicNumber % colorTable.size()).data();
}
Atom::~Atom() {

}


Molecule::Molecule() {

}
Molecule::~Molecule() {

}
bool Molecule::load(const char* inData, const char* inType) {
	atoms.clear(); bonds.clear();
	stringstream ss(inData), liness;
	string line, tmps, tmps2;
	int a1, a2, a3;
	float f1, f2, f3;
	if (QString(inType).toLower() == "mol2") {
		bool isAtomLine(false), isBondLine(false);
		while (getline(ss, line)) {
			if (line.length() < 3)
				continue;
			if (!isAtomLine && line.find("ATOM") != string::npos) {
				isAtomLine = true;
				continue;
			}
			if (!isBondLine && line.find("BOND") != string::npos) {
				isBondLine = true;
				continue;
			}
			if (isBondLine) {
				liness.str(line);
				liness >> tmps >> a1 >> a2 >> a3;
				addBond(a1 - 1, a2 - 1, a3 + 0.001);
				liness.clear();
			}
			else if (isAtomLine) {
				liness.str(line);
				liness >> tmps >> tmps;
				liness >> f1 >> f2 >> f3;
				addAtom(f1, f2, f3, tmps);
			}
		}
	}
	else if (QString(inType).toLower() == "pdb") {
		while (getline(ss, line)) {
			if (line.find("HETATM") != string::npos || line.find("ATOM") != string::npos) {
				liness.str(line);
				liness >> tmps >> tmps >> tmps;
				liness >> tmps2 >> tmps2;
				liness >> f1 >> f2 >> f3;
				addAtom(f1, f2, f3, tmps);
			}
			else if (line.find("CONECT") != string::npos) {
				liness.str(line);
				liness >> tmps >> a1 >> a2;
				while (!liness.eof()) {
					addBond(a1 - 1, a2 - 1);
					liness >> a2;
				}
				liness.clear();
			}
		}
	}
	else {
		return false;
	}
	return true;
}
void  Molecule::addBond(int _fromId, int _toId, float _level) {
	if (_fromId > _toId) {
		swap(_fromId, _toId);
	}
	for (auto& i : bonds) {
		if (i.aids.at(0) == _fromId && i.aids.at(1) == _toId) {
			return;
		}
	}
	bonds.emplace_back(Bond(_fromId, _toId, _level));
}
void Molecule::addAtom(float x, float y, float z, std::string& _elementName) {
	if (_elementName.size() > 1) {
		for (size_t i = 1; i < _elementName.size(); i++) {
			_elementName.at(i) = tolower(_elementName.at(i));
		}
	}
	//atoms.emplace_back(Atom(Point3D(x, y, z), _elementName));
	atoms.emplace_back(Atom(Vector3d(x, y, z), _elementName));
}
void Molecule::display() {
	for (size_t i = 0; i < atoms.size(); i++) {
		//cout << i << ": " << atoms.at(i).elementName << ".center = " << atoms.at(i).center << endl;
		cout << i << ": " << atoms.at(i).elementName << ".center = \n[" << atoms.at(i).center << "]\n";
	}
	for (size_t i = 0; i < bonds.size(); i++) {
		cout << i << ":";
		for (auto& j : bonds.at(i).aids) {
			cout << "-" << j << "-";
		}
		cout << ",¼ü¼¶:" << (int)bonds.at(i).level << endl;
	}
}
size_t Molecule::getAtomSize() const {
	return atoms.size();
}
size_t Molecule::getBondSize()const {
	return bonds.size();
}
const Atom& Molecule::getAtom(size_t i)const {
	if (i >= atoms.size())
		return atoms.at(0);
	return atoms.at(i);
}
const Bond& Molecule::getBond(size_t i) const {
	if (i >= bonds.size())
		return bonds.at(0);
	return bonds.at(i);
}