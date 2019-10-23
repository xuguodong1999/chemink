#ifndef ELEMENT_H
#define ELEMENT_H
#include"config.h"
#include"Fraction.h"
class Element {
public:
	Element();
	Element(std::string& _name);
	~Element();
	std::string name;
	int amount;
	Fraction valence;
	void clear();
	bool Exist();
	bool Reduced;
	Fraction finalValence;
};
#endif
