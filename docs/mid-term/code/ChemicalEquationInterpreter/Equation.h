#ifndef EQUATION_H
#define EQUATION_H
#include"config.h"
#include"Matter.h"
class Equation {
public:
	Equation();
	~Equation();
	std::string content;
	void clear();
	void inputContent();
	std::string p();
	bool IsConversed(std::vector<int>& raw);
	std::vector<int> oRatioList;
	std::vector<int> ratioList;
	bool exec(std::string& _content);
private:
	std::string equation;
	bool ie;
	std::vector<Matter> mRList;
	std::vector<Matter> mPList;
	std::vector<Element> eRList;
	std::vector<Element> ePList;
	std::vector<Fraction> redox;
	bool checkFormat();
	void split();
	bool intoMatter();
	bool intoElement();
	void getState();
	void addStateInfo();
	bool balance();
	bool getRedox();
	int getEleIndex(std::string& tarelenam);
};
#endif 
