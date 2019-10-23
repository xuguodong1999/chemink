#ifndef GROUP_H
#define GROUP_H

#include "Element.h"

class Group {
public:
	Group();
	~Group();
	void clear();
	std::string name;	//基团名称
	int amount;		//基团数量
	int q;			//基团电量
	std::vector<Element> eleList;
};
#endif //GROUP_H
