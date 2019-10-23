
/**

#include "Equation.h"

int main() {
	Equation e;
	std::string content;
	while (getline(std::cin,content)) {
		if (!e.exec(content)) {
			std::cout << "配平失败！" << std::endl;
		}
		e.clear();
		printMemory();
	}
	return 0;
}
/**/
