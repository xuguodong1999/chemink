#include "charinfo.h"

CharInfo::CharInfo():k(1.0){

}

void CharInfo::set(const vector<vector<pair<int, int>>>& script, string& _label) {
	label = _label.c_str();
	QPainterPath& result = path;
	for (auto& i : script) {
		QPainterPath tmp;
		tmp.moveTo(i.begin()->first, i.begin()->second);
		for (auto& j : i) {
			tmp.lineTo(j.first, j.second);
		}
		result.addPath(tmp);
	}
}
void CharInfo::collectRect() {
	
}

void CharBlock::setImgPos(const QPoint& p){
	imgPos = p;
	for (auto& i : b) {
		i.imgPos = imgPos;
	}
}

void CharBlock::append(const CharInfo& newChar){
	CharInfo tmp = newChar;
	if (b.isEmpty()) {
		b.append(newChar);
		
	}
	else {

	}
}
