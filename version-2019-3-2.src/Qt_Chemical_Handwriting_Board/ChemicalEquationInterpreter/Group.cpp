#include "Group.h"

Group::Group() {
	amount = 0; q = 0;
}

Group::~Group() {
	clear();
}

void Group::clear() {
	name.clear(); amount = 0; q = 0; eleList.clear();
}

