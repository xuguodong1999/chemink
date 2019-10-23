#include "glcheminfo.h"
#include <sstream>
#include <cmath>
using namespace std;

inline float vectorialModulus(const float* vec) {
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

inline float vectorialAngle(const float* A, const float* B) {
	float prod = A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
	float mod1 = vectorialModulus(A);
	float mod2 = vectorialModulus(B);
	const float PI = acos(-1.0f);
	float rad = acos(prod / (mod1 * mod2));
	float deg = rad * 180 / PI;
	return deg;
}

inline void vectorialProduct(const float* A, const float* B, float* prod) {
	prod[0] = A[1] * B[2] - A[2] * B[1];
	prod[1] = -(A[0] * B[2] - A[2] * B[0]);
	prod[2] = A[0] * B[1] - A[1] * B[0];
}

GlBond::GlBond() :a(0), b(0), bondLevel(1.001) { ; }

GlBond::GlBond(int _a, int _b, float _bondLevel) : a(_a), b(_b), bondLevel(_bondLevel) { ; }

GlPoint3D::GlPoint3D() {
	coordinate[0] = 0;
	coordinate[1] = 0;
	coordinate[2] = 0;
}

GlPoint3D::GlPoint3D(float x, float y, float z) {
	coordinate[0] = x;
	coordinate[1] = y;
	coordinate[2] = z;
}

float& GlPoint3D::x() { return coordinate[0]; }

float& GlPoint3D::y() { return coordinate[1]; }

float& GlPoint3D::z() { return coordinate[2]; }

float GlPoint3D::getRotationAngle(GlPoint3D& a, GlPoint3D& b, float* v) {
	static const float zAxis[3] = { 0.0, 0.0, 1.0 };
	static float diff[3];

	diff[0] = b.x() - a.x();
	diff[1] = b.y() - a.y();
	diff[2] = b.z() - a.z();

	float angle = vectorialAngle(zAxis, diff);
	vectorialProduct(zAxis, diff, v);

	return angle;
}

float GlPoint3D::getDistance(GlPoint3D& a, GlPoint3D& b) {
    return sqrt(pow(a.x() - b.x(), 2) + pow(a.y() - b.y(), 2) + pow(a.z() - b.z(), 2));
}

GlAtomInfo::GlAtomInfo() :
	atomicNumber(0), covalentRadius(0) {
	;
}

GlAtomInfo::GlAtomInfo(int _atomicNumber, float _covalentRadius) :
	atomicNumber(_atomicNumber), covalentRadius(_covalentRadius) {
	;
}

const map<string, GlAtomInfo> GlAtom::castTable = {
		pair<string,GlAtomInfo>("H",GlAtomInfo(1,32)),
		pair<string,GlAtomInfo>("He",GlAtomInfo(2,93)),
		pair<string,GlAtomInfo>("Li",GlAtomInfo(3,123)),
		pair<string,GlAtomInfo>("Be",GlAtomInfo(4,90)),
		pair<string,GlAtomInfo>("B",GlAtomInfo(5,82)),
		pair<string,GlAtomInfo>("C",GlAtomInfo(6,77)),
		pair<string,GlAtomInfo>("N",GlAtomInfo(7,75)),
		pair<string,GlAtomInfo>("O",GlAtomInfo(8,73)),
		pair<string,GlAtomInfo>("F",GlAtomInfo(9,72)),
		pair<string,GlAtomInfo>("Ne",GlAtomInfo(10,71)),
		pair<string,GlAtomInfo>("Na",GlAtomInfo(11,154)),
		pair<string,GlAtomInfo>("Mg",GlAtomInfo(12,136)),
		pair<string,GlAtomInfo>("Al",GlAtomInfo(13,118)),
		pair<string,GlAtomInfo>("Si",GlAtomInfo(14,111)),
		pair<string,GlAtomInfo>("P",GlAtomInfo(15,106)),
		pair<string,GlAtomInfo>("S",GlAtomInfo(16,102)),
		pair<string,GlAtomInfo>("Cl",GlAtomInfo(17,99)),
		pair<string,GlAtomInfo>("Ar",GlAtomInfo(18,98)),
		pair<string,GlAtomInfo>("K",GlAtomInfo(19,203)),
		pair<string,GlAtomInfo>("Ca",GlAtomInfo(20,174)),
		pair<string,GlAtomInfo>("Sc",GlAtomInfo(21,144)),
		pair<string,GlAtomInfo>("Ti",GlAtomInfo(22,132)),
		pair<string,GlAtomInfo>("V",GlAtomInfo(23,122)),
		pair<string,GlAtomInfo>("Cr",GlAtomInfo(24,118)),
		pair<string,GlAtomInfo>("Mn",GlAtomInfo(25,117)),
		pair<string,GlAtomInfo>("Fe",GlAtomInfo(26,117)),
		pair<string,GlAtomInfo>("Co",GlAtomInfo(27,116)),
		pair<string,GlAtomInfo>("Ni",GlAtomInfo(28,115)),
		pair<string,GlAtomInfo>("Cu",GlAtomInfo(29,117)),
		pair<string,GlAtomInfo>("Zn",GlAtomInfo(30,125)),
		pair<string,GlAtomInfo>("Ga",GlAtomInfo(31,126)),
		pair<string,GlAtomInfo>("Ge",GlAtomInfo(32,122)),
		pair<string,GlAtomInfo>("As",GlAtomInfo(33,120)),
		pair<string,GlAtomInfo>("Se",GlAtomInfo(34,116)),
		pair<string,GlAtomInfo>("Br",GlAtomInfo(35,114)),
		pair<string,GlAtomInfo>("Kr",GlAtomInfo(36,112)),
		pair<string,GlAtomInfo>("Rb",GlAtomInfo(37,216)),
		pair<string,GlAtomInfo>("Sr",GlAtomInfo(38,191)),
		pair<string,GlAtomInfo>("Y",GlAtomInfo(39,162)),
		pair<string,GlAtomInfo>("Zr",GlAtomInfo(40,145)),
		pair<string,GlAtomInfo>("Nb",GlAtomInfo(41,134)),
		pair<string,GlAtomInfo>("Mo",GlAtomInfo(42,130)),
		pair<string,GlAtomInfo>("Tc",GlAtomInfo(43,127)),
		pair<string,GlAtomInfo>("Ru",GlAtomInfo(44,125)),
		pair<string,GlAtomInfo>("Rh",GlAtomInfo(45,125)),
		pair<string,GlAtomInfo>("Pd",GlAtomInfo(46,128)),
		pair<string,GlAtomInfo>("Ag",GlAtomInfo(47,134)),
		pair<string,GlAtomInfo>("Cd",GlAtomInfo(48,148)),
		pair<string,GlAtomInfo>("In",GlAtomInfo(49,144)),
		pair<string,GlAtomInfo>("Sn",GlAtomInfo(50,141)),
		pair<string,GlAtomInfo>("Sb",GlAtomInfo(51,140)),
		pair<string,GlAtomInfo>("Te",GlAtomInfo(52,136)),
		pair<string,GlAtomInfo>("I",GlAtomInfo(53,133)),
		pair<string,GlAtomInfo>("Xe",GlAtomInfo(54,131)),
		pair<string,GlAtomInfo>("Cs",GlAtomInfo(55,235)),
		pair<string,GlAtomInfo>("Ba",GlAtomInfo(56,198)),
		pair<string,GlAtomInfo>("La",GlAtomInfo(57,169)),
		pair<string,GlAtomInfo>("Ce",GlAtomInfo(58,165)),
		pair<string,GlAtomInfo>("Pr",GlAtomInfo(59,165)),
		pair<string,GlAtomInfo>("Nd",GlAtomInfo(60,164)),
		pair<string,GlAtomInfo>("Pm",GlAtomInfo(61,163)),
		pair<string,GlAtomInfo>("Sm",GlAtomInfo(62,162)),
		pair<string,GlAtomInfo>("Eu",GlAtomInfo(63,185)),
		pair<string,GlAtomInfo>("Gd",GlAtomInfo(64,161)),
		pair<string,GlAtomInfo>("Tb",GlAtomInfo(65,159)),
		pair<string,GlAtomInfo>("Dy",GlAtomInfo(66,159)),
		pair<string,GlAtomInfo>("Ho",GlAtomInfo(67,158)),
		pair<string,GlAtomInfo>("Er",GlAtomInfo(68,157)),
		pair<string,GlAtomInfo>("Tm",GlAtomInfo(69,156)),
		pair<string,GlAtomInfo>("Yb",GlAtomInfo(70,174)),
		pair<string,GlAtomInfo>("Lu",GlAtomInfo(71,156)),
		pair<string,GlAtomInfo>("Hf",GlAtomInfo(72,144)),
		pair<string,GlAtomInfo>("Ta",GlAtomInfo(73,134)),
		pair<string,GlAtomInfo>("W",GlAtomInfo(74,130)),
		pair<string,GlAtomInfo>("Re",GlAtomInfo(75,128)),
		pair<string,GlAtomInfo>("Os",GlAtomInfo(76,126)),
		pair<string,GlAtomInfo>("Ir",GlAtomInfo(77,127)),
		pair<string,GlAtomInfo>("Pt",GlAtomInfo(78,130)),
		pair<string,GlAtomInfo>("Au",GlAtomInfo(79,134)),
		pair<string,GlAtomInfo>("Hg",GlAtomInfo(80,149)),
		pair<string,GlAtomInfo>("Tl",GlAtomInfo(81,148)),
		pair<string,GlAtomInfo>("Pb",GlAtomInfo(82,147)),
		pair<string,GlAtomInfo>("Bi",GlAtomInfo(83,146)),
		pair<string,GlAtomInfo>("Po",GlAtomInfo(84,146)),
		pair<string,GlAtomInfo>("At",GlAtomInfo(85,145)),
		pair<string,GlAtomInfo>("Th",GlAtomInfo(90,165)),
		pair<string,GlAtomInfo>("U",GlAtomInfo(92,142))
};

GlAtom::GlAtom() { ; }

int GlAtom::getAtomicNumber(const char* name) {
	return castTable.at(name).atomicNumber;
}

float GlAtom::getCovalentRadius(const char* name) {
	return castTable.at(name).covalentRadius;
}

GlMol::GlMol() {

}

GlMol::~GlMol() {

}

void GlMol::loadPdb(const char* data) {
	links.clear();
	nodes.clear();
	stringstream ss, liness;
	string line, tmp;
	int a1, a2;
	ss.str(data);
	while (getline(ss, line)) {
		if (line.find("HETATM") != string::npos || line.find("ATOM") != string::npos) {
			GlAtom glatom;
			liness.str(line);
			liness >> tmp >> tmp >> tmp;
			glatom.atomicNumber = glatom.getAtomicNumber(tmp.c_str());
			glatom.covalentRadius = glatom.getCovalentRadius(tmp.c_str());
			liness >> tmp >> tmp;
			liness >> glatom.x() >> glatom.y() >> glatom.z();
			nodes.push_back(glatom);
		}
		else if (line.find("CONECT") != string::npos) {
			liness.str(line);
			liness >> tmp >> a1 >> a2;
			while (!liness.eof()) {
				addBond(a1 - 1, a2 - 1);//  pdb格式外显键级全部为一，pdb原子编号从1开始
				liness >> a2;
			}
			liness.clear();
		}
	}
}

void GlMol::addBond(int a, int b, float bondLevel) {
	if (a > b) {
		int tmp = a;
		a = b;
		b = tmp;
	}
	for (auto i : links) {
		if (i.a == a && i.b == b) {
			return;
		}
	}
	links.push_back(GlBond(a, b, bondLevel));
}

void GlMol::loadMol2(const char* data) {
	links.clear();
	nodes.clear();
	stringstream ss, liness;
	string line, tmp;
	int a1, a2, a3;
	ss.str(data);
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
			liness >> tmp >> a1 >> a2 >> a3;
			addBond(a1 - 1, a2 - 1, a3 + 0.001);//  mol2格式表达了键级
			liness.clear();
		}
		else if (isAtomLine) {
			GlAtom glatom;
			liness.str(line);
			liness >> tmp >> tmp;
			glatom.atomicNumber = glatom.getAtomicNumber(tmp.c_str());
			glatom.covalentRadius = glatom.getCovalentRadius(tmp.c_str());
			liness >> glatom.x() >> glatom.y() >> glatom.z();
			nodes.push_back(glatom);
		}
	}
}

GlBond& GlMol::bond(int id) { return links.at(id); }

GlAtom& GlMol::atom(int id) { return nodes.at(id); }

size_t GlMol::getBondNum() {
	return links.size();
}

size_t GlMol::getAtomNum() {
	return nodes.size();
}

GlBondLink::GlBondLink() { ; }

GlBondLink::~GlBondLink() { ; }
