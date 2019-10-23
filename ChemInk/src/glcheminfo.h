#pragma once
#include <string>
#include <map>
#include <vector>

class GlBond {
public:
	int a, b;
	float bondLevel;
	GlBond();
	GlBond(int _a, int _b, float _bondLevel = 1.001);
};

class GlPoint3D {
public:
	GlPoint3D();
	GlPoint3D(float x, float y, float z);
	float& x();
	float& y();
	float& z();
	static float getRotationAngle(GlPoint3D& a, GlPoint3D& b, float* v);
	static float getDistance(GlPoint3D& a, GlPoint3D& b);
private:
	float   coordinate[3];
};

class GlAtomInfo :public GlPoint3D {
public:
	int atomicNumber;
	float covalentRadius;
	GlAtomInfo();
	GlAtomInfo(int _atomicNumber, float _covalentRadius);
};

class GlAtom :public GlAtomInfo {
public:
	GlAtom();
	int getAtomicNumber(const char* name);
	float getCovalentRadius(const char* name);
private:
	static const std::map<std::string, GlAtomInfo> castTable;
};

class GlMol {
public:
	GlMol();
	~GlMol();
	void loadPdb(const char* data);
	void loadMol2(const char* data);
	GlBond& bond(int id);
	GlAtom& atom(int id);
	size_t getBondNum();
	size_t getAtomNum();
private:
	std::vector<GlBond> links;
	std::vector<GlAtom> nodes;
	void addBond(int a, int b, float bondLevel = 1.001);
};

class GlBondLink {
public:
	GlBondLink();
	~GlBondLink();
	int         atomId;
	float       height;
	int			stickCounter;
	typedef float GLfloat;
	GLfloat     rotVec[3];
	float       angle;
};
