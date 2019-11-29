#pragma once

#include "stdafx.h"
#include <string>
class ChemBalancer {
public:
	ChemBalancer();
	~ChemBalancer();
	std::string addHtml(std::string& input);
	std::string balance(std::string& input);
};


