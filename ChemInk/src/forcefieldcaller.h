#pragma once
#include "threadsupport.h"
#include <string>
class ForceFieldCaller:public ThreadSupport {
private:
	std::string forcefield, inputformat, outputformat;
public:
	ForceFieldCaller();
	~ForceFieldCaller();
	void setForceField(const char* _forcefield = "UFF");
	void setInputFormat(const char* _inputformat = "SMILES");
	void setOutputFormat(const char* _outputformat = "MOL2");
	void smiles2coordinates(const char* inputstring,std::string& outputs);
private:
	void hackabug();//  多做一次清掉力场残留

protected:
	void load()override;
	void predict()override;
};

