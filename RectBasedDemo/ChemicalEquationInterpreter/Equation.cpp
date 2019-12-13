#include "Equation.h"
Equation::Equation() {
	ie = 0;
}
Equation::~Equation() {
	clear();
}
void Equation::clear() {
	ie = 0;				redox.clear();
	equation.clear();	content.clear();
	mRList.clear();		mPList.clear();
	eRList.clear();		ePList.clear();
	ratioList.clear();	oRatioList.clear();
}
void Equation::inputContent() {
	getline(std::cin, content);
}
void Equation::getState() {
	for (int i = 0; i < (int)mRList.size(); i++)
		mRList[i].getStateInfo();
	for (int i = 0; i < (int)mPList.size(); i++)
		mPList[i].getStateInfo();
}
bool Equation::checkFormat() {
	deleteAllMark(content, " ");
	int len = (int)content.length();
	if (len < 3)
		return 0;
	if (content[0] == '=' || content[len - 1] == '=')
		return 0;
	int tmp = 0;
	for (int i = 1; i < len - 1; i++) {
		if (content[i] == '=') {
			tmp++;
			while (content[i] == '=')
				i++;
		}
	}
	if (tmp != 1)
		return 0;
	return 1;
}
void Equation::split() {
	int i = 0;
	char *ca = new char[MAXC];
	int index = 0;
	int len = (int)content.length();
	int start = 0;
	while (i < len) {
		if (content[i] < '0' || content[i] > '9') {
			oRatioList.push_back(1);
			while (i != len && content[i] != '+' && content[i] != '=')i++;
			i++;
		}
		else {
			start = i; index = 0;
			ca[index++] = content[i++];
			while (content[i] >= '0' && content[i] <= '9')
				ca[index++] = content[i++];
			ca[index] = '\0';
			oRatioList.push_back(atoi(ca));
			content.erase(start, index);
			len = (int)content.length();
			i = i - index;
			while (i != len && (content[i] != '+' && content[i] != '='))i++;
			i++;
		}
	}
	delete[]ca;
	equation = content;
}
bool Equation::intoMatter() {
	if (!checkFormat()) {
#ifdef TESTCEI
		std::cout << "不满足方程的格式：XXX=...=XXX！" << std::endl;
#endif
		return 0;
	}
	if (content.find("@") != std::string::npos || content.find("$") != std::string::npos)
		ie = 1;
	else
		ie = 0;
	split();
	int len = (int)equation.length(); int p = 0;
	Matter mat; bool is_reac = 1;
	while (p != len) {
		if (equation[p] == '+') {
			if (is_reac)
				mRList.push_back(mat);
			else
				mPList.push_back(mat);
			mat.clear();
		}
		else if (equation[p] == '=') {
			if (is_reac)
				mRList.push_back(mat);
			else
				mPList.push_back(mat);
			mat.clear();
			is_reac = 0;
			while (p != len - 1 && equation[p] == '=')p++;
			p--;
		}
		else if (p == len - 1) {
			mat.name += equation[p];
			if (is_reac)
				mRList.push_back(mat);
			else
				mPList.push_back(mat);
			mat.clear();
		}
		else
			mat.name += equation[p];
		p++;
	}
	if (!intoElement()) {
#ifdef TESTCEI
		std::cout << "字符串拆分失败！未完成元素级拆分。" << std::endl;
#endif
		return 0;
	}
	return 1;
}
void Equation::addStateInfo() {
	for (int i = 0; i < (int)mRList.size(); i++)
		mRList[i].getStateInfo();
	for (int i = 0; i < (int)mPList.size(); i++)
		mPList[i].getStateInfo();
	bool addUp = 1; bool addDown = 1;
	for (int i = 0; i < (int)mRList.size(); i++)
		if (mRList[i].state == 'g') {
			addUp = 0; break;
		}
	for (int i = 0; i < (int)mRList.size(); i++)
		if (mRList[i].state == 's' || mRList[i].state == 'm') {
			addDown = 0; break;
		}
	for (int i = 0; i < (int)mPList.size(); i++) {
		if (addUp &&  mPList[i].state == 'g')
			mPList[i].stateInfo = "(g)";
		else if (addDown &&  mPList[i].state == 's')
			mPList[i].stateInfo = "(s)";
	}
}
std::string Equation::p() {
	char* tmp = new char[MAXC]; std::string res = "<h4>"; i2a(ratioList[0], tmp, 10);
	if (ratioList[0] != 1)res += tmp;
	res += addHtml(mRList[0].name);
	int i = 0; int rs = (int)mRList.size();
	for (i = 1; i < rs; i++) {
		res += " + "; i2a(ratioList[i], tmp, 10);
		if (ratioList[i] != 1)res += tmp;
		res += addHtml(mRList[i].name);
	}
	res += "  =  "; i2a(ratioList[rs], tmp, 10);
	if (ratioList[rs] != 1)res += tmp;
	res = res + addHtml(mPList[0].name) + mPList[0].stateInfo;
	for (i = 1; i < (int)mPList.size(); i++) {
		res += " + "; i2a(ratioList[i + rs], tmp, 10);
		if (ratioList[i + rs] != 1)res += tmp;
		res = res + addHtml(mPList[i].name) + mPList[i].stateInfo;
	}
	delete[]tmp; res += "</h4>";
	return res;
}
bool Equation::intoElement() {
	for (int i = 0; i < (int)mRList.size(); i++)
		if (!mRList[i].intoElement()) {
#ifdef TESTCEI
			std::cout << "字串 " << mRList[i].name << " 解析失败！" << std::endl;
#endif
			return 0;
		}
	for (int i = 0; i < (int)mPList.size(); i++)
		if (!mPList[i].intoElement()) {
#ifdef TESTCEI
			std::cout << "字串 " << mPList[i].name << " 解析失败！" << std::endl;
#endif
			return 0;
		}
	for (int i = 0; i < (int)mRList.size(); i++) {
		for (int j = 0; j < (int)mRList[i].eleList.size(); j++)
			eRList.push_back(mRList[i].eleList[j]);
	}
	sort(eRList.begin(), eRList.end(), namelessthan);
	for (int i = 0; i < (int)eRList.size() - 1; i++) {
		int j = i + 1;
		while (j < (int)eRList.size() && eRList[i].name == eRList[j].name) {
			eRList[i].amount += eRList[j].amount;
			j++;
		}
		eRList.erase(eRList.begin() + i + 1, eRList.begin() + j);
	}
	for (int i = 0; i < (int)mPList.size(); i++) {
		for (int j = 0; j < (int)mPList[i].eleList.size(); j++)
			ePList.push_back(mPList[i].eleList[j]);
	}
	sort(ePList.begin(), ePList.end(), namelessthan);
	for (int i = 0; i < (int)ePList.size() - 1; i++) {
		int j = i + 1;
		while (j < (int)ePList.size() && ePList[i].name == ePList[j].name) {
			ePList[i].amount += ePList[j].amount;
			j++;
		}
		ePList.erase(ePList.begin() + i + 1, ePList.begin() + j);
	}
	return 1;
}
bool Equation::balance() {
	int row = (int)eRList.size();
	if (row != (int)ePList.size()) {
#ifdef TESTCEI
		std::cout << "元素种类不守恒，也许发生了核反应。退出配平！" << std::endl;
#endif
		return 0;
	}
	int i = 0, j = 0, cr = (int)mRList.size();
	int col = cr + (int)mPList.size();
	int*a = new int[row * col];
	for (i = 0; i < row; i++) {
		for (j = 0; j < cr; j++)
			a[col * i + j] = mRList[j].getEleAmount(eRList[i].name);
		for (j = cr; j < col; j++)
			a[col * i + j] = -mPList[j - cr].getEleAmount(eRList[i].name);
	}
#ifdef TESTCEI
	std::cout << std::endl << "元素守恒系数矩阵：" << std::endl;
	std::cout << "     o-";
	for (j = 0; j <= col; j++)
		std::cout << "    ";
	std::cout << "-o" << std::endl;
	for (i = 0; i < row; i++) {
		std::cout << "     |";
		for (j = 0; j < col; j++)
			printf("%4d", a[i*col + j]);
		std::cout << "      |" << std::endl;
	}
	std::cout << "     o_";
	for (j = 0; j <= col; j++)
		std::cout << "    ";
	std::cout << "_o" << std::endl << std::endl;
#endif
	int resint[MAXM];
	int matrix1[MAXM][MAXM];
	int index_a = 0;
	for (i = 1; i <= row; i++) {
		for (j = 1; j <= col; j++)
			matrix1[i][j] = a[index_a++];
	}
	Matrix_row_transformation(matrix1, row, col);
	if (row <= col - 2) {
		if (!ie) {
			redox.clear(); Fraction zero = 0;
			for (i = 0; i < col; i++)
				redox.push_back(zero);
			if (!getRedox()) {
#ifdef TESTCEI
				std::cout << "无法获得化合价信息以构造方程组，退出配平！" << std::endl;
#endif
				delete[]a; return 0;
			}
			row++;
			for (i = 1; i <= col; i++)
				matrix1[row][i] = redox[i - 1].a;
		}
		else {
			row++;
			for (j = 0; j < cr; j++)
				matrix1[row][j + 1] = mRList[j].q;
			for (j = cr; j < col; j++)
				matrix1[row][j + 1] = -mPList[j - cr].q;
		}
	}
#ifdef TESTCEI
	std::cout << std::endl << "最终系数矩阵：" << std::endl << "     o-";
	for (j = 0; j <= col; j++)std::cout << "    ";
	std::cout << "-o" << std::endl;
	for (i = 0; i < row; i++) {
		std::cout << "     |";
		for (j = 0; j < col; j++)printf("%4d", matrix1[i + 1][j + 1]);
		std::cout << "      |" << std::endl;
	}
	std::cout << "     o_";
	for (j = 0; j <= col; j++)std::cout << "    ";
	std::cout << "_o" << std::endl << std::endl;
#endif
	if (row <= col - 2) {
#ifdef TESTCEI
		std::cout << "存在多解，需要更多信息，配平失败！" << std::endl;
#endif
		delete[]a; return 0;
	}
	bool success = Gauss(matrix1, row, col, resint);
	if (!success) {
#ifdef TESTCEI
		std::cout << "无法进行高斯消元，配平出错！" << std::endl;
#endif
		delete[]a; return 0;
	}
	for (i = 1; i <= col; i++) {
		if (resint[i] <= 0) {
			matrix1[row][1] = 1; matrix1[row][2] = -1;
			for (i = 3; i <= col; i++)
				matrix1[row][i] = 0;
			success = Gauss(matrix1, row, col, resint);
			if (!success) {
#ifdef TESTCEI
				std::cout << "无法进行高斯消元，配平出错！" << std::endl;
#endif
				delete[]a; return 0;
			}
			success = 1;
			for (j = 1; j <= col; j++) {
				if (resint[j] <= 0) {
					success = 0; break;
				}
			}
			if (success) {
				ratioList.clear();
				for (j = 1; j <= col; j++)
					ratioList.push_back(resint[j]);
				delete[] a; return 1;
			}
#ifdef TESTCEI
			std::cout << "高斯消元完成后出现负系数，指定系数依然无效，配平出错！" << std::endl;
#endif
			delete[]a; return 0;
		}
		ratioList.push_back(resint[i]);
	}
	delete[]a;
	return 1;
}
bool Equation::getRedox() {
	int cr = (int)mRList.size(), cp = (int)mPList.size();
	int i = 0, j = 0, i0 = 0, j0 = 0;
	for (i = 0; i < cr; i++)
		if (!mRList[i].getValence()) {
#ifdef TESTCEI
			std::cout << "无法获得 " << mRList[i].name << " 的化合价，配平失败！" << std::endl;
#endif
			return 0;
		}
	for (i = 0; i < cp; i++)
		if (!mPList[i].getValence()) {
#ifdef TESTCEI
			std::cout << "无法获得 " << mPList[i].name << " 的化合价，配平失败！" << std::endl;
#endif
			return 0;
		}
	for (i = 0; i < cr; i++) {
		for (j = 0; j < (int)mRList[i].eleList.size(); j++) {
			for (i0 = 0; i0 < cp; i0++)
				for (j0 = 0; j0 < (int)mPList[i0].eleList.size(); j0++)
					if (mRList[i].eleList[j].name == mPList[i0].eleList[j0].name && mRList[i].eleList[j].valence != mPList[i0].eleList[j0].valence) {
						mRList[i].eleList[j].Reduced = 1; goto BREAKPOINT1;
					}
		BREAKPOINT1:;
		}
	}
	for (i = 0; i < cp; i++) {
		for (j = 0; j < (int)mPList[i].eleList.size(); j++) {
			for (i0 = 0; i0 < cr; i0++)
				for (j0 = 0; j0 < (int)mRList[i0].eleList.size(); j0++)
					if (mPList[i].eleList[j].name == mRList[i0].eleList[j0].name && mPList[i].eleList[j].valence != mRList[i0].eleList[j0].valence) {
						mPList[i].eleList[j].Reduced = 1; goto BREAKPOINT2;
					}
		BREAKPOINT2:;
		}
	}
	for (i = 0; i < cr; i++) {
		for (j = 0; j < (int)mRList[i].eleList.size(); j++) {
			for (i0 = 0; i0 < cp; i0++)
				for (j0 = 0; j0 < (int)mPList[i0].eleList.size(); j0++)
					if (mRList[i].eleList[j].name == mPList[i0].eleList[j0].name && mRList[i].eleList[j].valence == mPList[i0].eleList[j0].valence) {
						mRList[i].eleList[j].Reduced = 0; goto BREAKPOINT3;
					}
		BREAKPOINT3:;
		}
	}
	for (i = 0; i < cp; i++) {
		for (j = 0; j < (int)mPList[i].eleList.size(); j++) {
			for (i0 = 0; i0 < cr; i0++)
				for (j0 = 0; j0 < (int)mRList[i0].eleList.size(); j0++)
					if (mPList[i].eleList[j].name == mRList[i0].eleList[j0].name && mPList[i].eleList[j].valence == mRList[i0].eleList[j0].valence) {
						mPList[i].eleList[j].Reduced = 0; goto BREAKPOINT4;
					}
		BREAKPOINT4:;
		}
	}
	for (i = 0; i < cr; i++) {
		for (j = 0; j < (int)mRList[i].eleList.size(); j++)
			for (i0 = 0; i0 < cp; i0++)
				for (j0 = 0; j0 < (int)mPList[i0].eleList.size(); j0++)
					if (mRList[i].eleList[j].name == mPList[i0].eleList[j0].name && mRList[i].eleList[j].valence != mPList[i0].eleList[j0].valence && mPList[i0].eleList[j0].Reduced == 1)
						if ((Abs(mRList[i].eleList[j].finalValence - mRList[i].eleList[j].valence) > Abs(mPList[i0].eleList[j0].valence - mRList[i].eleList[j].valence)))
							mRList[i].eleList[j].finalValence = mPList[i0].eleList[j0].valence;
	}
	Fraction tmp;
	for (i = 0; i < cr; i++) {
		for (j = 0; j < (int)mRList[i].eleList.size(); j++)
			if (mRList[i].eleList[j].Reduced) {
				tmp.a = mRList[i].eleList[j].amount; tmp.b = 1;
				redox[i] = redox[i] + tmp * (mRList[i].eleList[j].valence - mRList[i].eleList[j].finalValence);
			}
	}
#ifdef TESTCEI
	std::cout << "氧化还原守恒向量：[";
	for (int i = 0; i < cr + cp; i++)
		printf("%4d", redox[i].a);
	std::cout << "      ]" << std::endl;
#endif
	return 1;
}
bool Equation::IsConversed(std::vector<int>& raw) {
	int cer = (int)eRList.size(), cep = (int)ePList.size();
	int cr = (int)mRList.size(), cp = (int)mPList.size();
	if (cer != cep) {
		return 0;
	}
    int i = 0, j = 0;
	for (i = 0; i < cer; i++) {
		if (eRList[i].name != ePList[i].name)
			return 0;
	}
	int * check = new int[MAXM];
	for (i = 0; i < MAXM; i++)check[i] = 0;
	for (i = 0; i < cr; i++) {
		for (j = 0; j < (int)mRList[i].eleList.size(); j++)
			check[getEleIndex(mRList[i].eleList[j].name)] += raw[i] * mRList[i].eleList[j].amount;
	}
	for (i = 0; i < cp; i++) {
		for (j = 0; j < (int)mPList[i].eleList.size(); j++)
			check[getEleIndex(mPList[i].eleList[j].name)] -= raw[i + cr] * mPList[i].eleList[j].amount;
	}
	for (i = 0; i < cer; i++) {
		if (check[i] != 0) {
			delete[]check;
			return 0;
		}
	}
	delete[] check;
	return 1;
}
int Equation::getEleIndex(std::string& tarelenam) {
	for (int i = 0; i < (int)eRList.size(); i++)
		if (tarelenam == eRList[i].name)
			return i;
	return -1;
}
bool Equation::exec(std::string& _content) {
	content = _content;
	if (!intoMatter()) {
#ifdef TESTCEI
		std::cout << "字符串拆分失败！未完成物质级拆分。" << std::endl;
#endif
		return 0;
	}
	if (!balance()) {
#ifdef TESTCEI
		std::cout << "方程式配平失败！" << std::endl;
#endif
		return 0;
	}
	addStateInfo();
	if (!IsConversed(ratioList)) {
#ifdef TESTCEI
		std::cout << "机器配平错误！" << std::endl;
#endif
		return 0;
	}
	//std::cout << p() << std::endl;
	return 1;
}
