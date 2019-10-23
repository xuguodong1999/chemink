#include "forcefieldcaller.h"

#include <openbabel/babelconfig.h>
#include <openbabel/base.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/forcefield.h>
#include <openbabel/builder.h>

using namespace std;
using namespace OpenBabel;

static OBConversion conv;
static OBFormat* format_in(nullptr);
static OBFormat* format_out(nullptr);
static OBForceField* obff(nullptr);

ForceFieldCaller::ForceFieldCaller() :forcefield(), inputformat(), outputformat() {
	
}

ForceFieldCaller::~ForceFieldCaller() {

}

void ForceFieldCaller::setForceField(const char* _forcefield) {
	if (obff != nullptr)
		delete obff;
	obff = (OBForceField*)OBPlugin::GetPlugin("forcefields", _forcefield);
	if (!obff) 
		cout << "加载力场失败" << endl;
	else {
		cout << "加载力场成功" << endl;
		forcefield = _forcefield;
	}

}

void ForceFieldCaller::setInputFormat(const char* _inputformat) {
	if (!format_in)
		delete format_in;
	inputformat = _inputformat;
	format_in = conv.FormatFromExt(_inputformat);
}

void ForceFieldCaller::setOutputFormat(const char* _outputformat) {
	if (!format_out)
		delete format_out;
	outputformat = _outputformat;
	format_out = conv.FindFormat(_outputformat);
	if (format_in && format_out) {
		cout << "格式信息加载成功" << endl;
		conv.SetInAndOutFormats(format_in, format_out);
	}
	else {
		if (!format_in)
			cout << "输入格式加载失败" << endl;
		if (!format_out)
			cout << "输出格式加载失败" << endl;
	}
}

void ForceFieldCaller::smiles2coordinates(const char* inputstring, std::string& outputs) {
	if (!(format_in && format_out && obff)) {
		outputs = "[ERROR] 格式或力场没有完全加载成功"; 
		cout << outputs << endl; return;
	}
	OBMol mol;
	stringstream ism(inputstring);
	conv.Read(&mol, &ism);
	if (mol.Empty()) {
		outputs = "[ERROR] 输入字串解析失败"; 
		cout << outputs << endl; return;
	}
	OBBuilder builder;
	builder.Build(mol);
	mol.AddHydrogens(false, true);
	if (!obff->Setup(mol)) {
		outputs = "[ERROR] 力场初始化失败";
		cout << outputs << endl; return;
	}
	int steps = 100;// mol.NumAtoms() * 10;
	obff->SteepestDescent(steps);
	obff->WeightedRotorSearch(steps /2 , steps/10);
	obff->SteepestDescent(steps);
	obff->UpdateCoordinates(mol);
	stringstream ss;
	conv.Write(&mol, &ss);
	outputs = ss.str();
	hackabug();
}
void ForceFieldCaller::load() {
	setInputFormat();
	setOutputFormat();
	setForceField();
}

void ForceFieldCaller::predict() {
	cout << "TODO: 把力场模拟任务加入额外线程" << endl;
}

void ForceFieldCaller::hackabug() {
	OBMol mol;
	stringstream ism("C");
	conv.Read(&mol, &ism);
	if (mol.Empty())
		return;
	OBBuilder builder;
	builder.Build(mol);
	obff->Setup(mol);
}