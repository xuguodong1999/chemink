#include "alkanetopo.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <ctime>
using namespace std;


void UnrootedTree::dfsCollectSizeOfSon(int x, int father) {		//  获取子树大小 找重心
	son1[x] = 1, son2[x] = 0;
	for (size_t i = 0; i < utree->at(x).size(); i++) {
		int p = utree->at(x).at(i);
		if (p == father)
			continue;
		dfsCollectSizeOfSon(p, x);
		son1[x] += son1[p];
		son2[x] = max(son2[x], son1[p]);
	}
	son2[x] = max(son2[x], n - son1[x]);
	nodeOfMaxSubTree = max(son2[x], nodeOfMaxSubTree);
}

void UnrootedTree::dfsGetSeq(int x, int father) {
	allSeq[x] = "(";
	for (size_t i = 0; i < utree->at(x).size(); i++) {
		int p = utree->at(x).at(i);
		if (p != father)
			dfsGetSeq(p, x);
	}
	int now = 0;
	for (size_t i = 0; i < utree->at(x).size(); i++) {
		int p = utree->at(x).at(i);
		if (p != father)
			partSeq[now++] = allSeq[p];
	}
	sort(partSeq.begin(), partSeq.begin() + now);
	for (int i = 0; i < now; i++)
		allSeq[x] += partSeq[i];
	allSeq[x] += ")";
}

UnrootedTree::UnrootedTree() :utree(nullptr), n(0), nodeOfMaxSubTree(-1) { ; }

UnrootedTree::~UnrootedTree() {
	utree = nullptr;
	son1.clear();		son2.clear();
	partSeq.clear();	allSeq.clear();
}

string UnrootedTree::getSeq(vector<vector<unsigned char>>& _utree) {	//  获取括号序列
	utree = &_utree;
	n = utree->size();							//  比碳原子数多一个 0,1-n
	son1.resize(n, 0);			son2.resize(n, 0);
	partSeq.resize(n, "");		allSeq.resize(n, "");
	nodeOfMaxSubTree = -1;						//  最大子树的节点数
	dfsCollectSizeOfSon(1, 0);
	string finalSeq = "";
	for (int i = 1; i <= n; i++) {				//  处理两个重心
		if (son2[i] == nodeOfMaxSubTree) {		//  等于最大子树的节点数
			dfsGetSeq(i, 0);
			if (allSeq[i] > finalSeq)
				finalSeq = allSeq[i];
		}
	}
	utree = nullptr;
	son1.clear();		son2.clear();
	partSeq.clear();	allSeq.clear();
	return finalSeq;
}

int UnrootedTree::getMaxSubTree() {
	return nodeOfMaxSubTree;
}

vector<int>* UnrootedTree::getSubTreeSize(vector<vector<unsigned char>>& _utree) {
	utree = &_utree;
	n = utree->size();							//  比碳原子数多一个 0,1-n
	son1.resize(n, 0);			son2.resize(n, 0);
	partSeq.resize(n, "");		allSeq.resize(n, "");
	nodeOfMaxSubTree = -1;						//  最大子树的节点数
	dfsCollectSizeOfSon(1, 0);
	return &son2;
}

void UnrootedTree::clearSubTreeSize() {
	son1.clear();
	son2.clear();
	utree = nullptr;
}

void UnrootedTree::display() {
	if (utree == nullptr) {
		cout << "未初始化邻接表，无法打印" << endl;
		return;
	}
	for (size_t i = 0; i < utree->size(); i++) {
		cout << i << " --> ";
		for (size_t j = 0; j < utree->at(i).size(); j++) {
			cout << utree->at(i).at(j) << ", ";
		}
		cout << endl;
	}
}

AlkaneTopo::AlkaneTopo() :lastStructs(nullptr), newStructs(nullptr), numberOfCarbon(1) {
	curStruct.clear();
	isomers.clear();
	isomerCounter.resize(2, 0);
	isomerCounter[1] = 1;
}

AlkaneTopo::~AlkaneTopo() {
	if (lastStructs != nullptr) {
		delete lastStructs;
		lastStructs = nullptr;
	}
	if (newStructs != nullptr) {
		delete newStructs;
		newStructs = nullptr;
	}
	curStruct.clear();
	isomers.clear();
	isomerCounter.clear();
}
static inline bool hasTargetNum(vector<int>& targetNum, int a) {
	for (int i : targetNum) {
		if (a == i)
			return true;
	}
	return false;
}
void AlkaneTopo::getAlkaneTopo() {
	if (numberOfCarbon == 1) {										//  一个结点
		newStructs = new vector<vector<vector<unsigned char>>>();
		newStructs->resize(2);										//  返回空邻接表
		isomerCounter[1] = 1;
		return;
	}
	else if (numberOfCarbon == 2) {									//  两个结点
		newStructs = new vector<vector<vector<unsigned char>>>();
		newStructs->resize(1);
		newStructs->at(0).resize(3);								//  C2的第一种也是唯一一种拓扑连接
		newStructs->at(0).at(1).push_back(2);						//  C1-C2
		newStructs->at(0).at(2).push_back(1);						//  C2-C1
		isomerCounter[2] = 1;
		return;
	}
	//  三个及以上结点，迭代计算
	lastStructs = new vector<vector<vector<unsigned char>>>();			//  为记录所有i-1个碳的结构开辟空间
	lastStructs->resize(1);										//  C2只有一个拓扑结构 碳原子编号从1开始
	lastStructs->at(0).resize(3);								//  C2的第一种也是唯一一种拓扑连接
	lastStructs->at(0).at(1).push_back(2);						//  C1-C2
	lastStructs->at(0).at(2).push_back(1);						//  C2-C1
	string tmp;
	clock_t start, end;
	for (int i = 3; i <= numberOfCarbon; i++) {
		start = clock();
		newStructs = new vector<vector<vector<unsigned char>>>();			//  为记录所有i个碳的新结构开辟空间										
		for (size_t j = 0; j < lastStructs->size(); j++) {		//  遍历i-1个碳的所有连接方式
			for (int k = 1; k <= i - 1; k++) {					//  遍历拓扑结构j上的所有碳原子
				if (lastStructs->at(j).at(k).size() > 3)		//  一个碳上最多连4个碳
					continue;
				curStruct.clear();
				curStruct = lastStructs->at(j);					//  记下原来的结构
				curStruct.resize(i + 1);
				curStruct.at(k).push_back(i);					//  把新原子连到k号碳
				curStruct.at(i).push_back(k);
				tmp = utreeFunc.getSeq(curStruct);				//  获取curStruct无根树的括号序列
				//if (isomers.find(tmp) == isomers.end()) {		//  是一个没有被记录过的结构 存入所有新结构中
				//	isomerCounter.at(i)++;						//  i个碳非光学同分异构体数量加一
				//	isomers[tmp] = newStructs->size();			//  加进新结构列表
				//	newStructs->push_back(curStruct);
				//}
				if (isomers.insert(tmp).second == true) {
					isomerCounter.at(i)++;
					newStructs->push_back(curStruct);
					cout << utreeFunc.getSMILES(curStruct) << endl;
				}
			}
		}
		cout << "i=" << i << ", count=" << isomerCounter.at(i) << endl;
		end = clock();
		double endtime = (double)(end - start) / CLOCKS_PER_SEC;
		cout << "生成完整拓扑结构用时:" << endtime << "秒" << endl;		//  s为单位
		start = end;
		isomers.clear();										//  清掉i个碳的所有合法括号序列
		delete lastStructs;										//  归还旧结构占用的空间
		lastStructs = newStructs;								//  旧指针转移
		newStructs = nullptr;
	}
}

void AlkaneTopo::getAlkaneTopo(vector<string>& result, vector<int>& targetCarbonNum) {
	setNumberOfCarbon(targetCarbonNum.back());
	if (numberOfCarbon == 1) {										//  一个结点
		newStructs = new vector<vector<vector<unsigned char>>>();
		newStructs->resize(2);										//  返回空邻接表
		isomerCounter[1] = 1;
		result.push_back("C");
		return;
	}
	else if (numberOfCarbon == 2) {									//  两个结点
		newStructs = new vector<vector<vector<unsigned char>>>();
		newStructs->resize(1);
		newStructs->at(0).resize(3);								//  C2的第一种也是唯一一种拓扑连接
		newStructs->at(0).at(1).push_back(2);						//  C1-C2
		newStructs->at(0).at(2).push_back(1);						//  C2-C1
		isomerCounter[2] = 1;
		return;
	}
	if (targetCarbonNum.at(0) == 1) {//  传入的数组必然有序
		result.push_back("C");
	}
	if (targetCarbonNum.at(0) == 2 ||
		(targetCarbonNum.size() > 1 && targetCarbonNum.at(1) == 2)) {
		result.push_back("C(C)");
	}
	//  三个及以上结点，迭代计算
	lastStructs = new vector<vector<vector<unsigned char>>>();			//  为记录所有i-1个碳的结构开辟空间
	lastStructs->resize(1);										//  C2只有一个拓扑结构 碳原子编号从1开始
	lastStructs->at(0).resize(3);								//  C2的第一种也是唯一一种拓扑连接
	lastStructs->at(0).at(1).push_back(2);						//  C1-C2
	lastStructs->at(0).at(2).push_back(1);						//  C2-C1
	string tmp;
	bool allowAdd = false;
	for (int i = 3; i <= numberOfCarbon; i++) {
		allowAdd = hasTargetNum(targetCarbonNum, i);
		newStructs = new vector<vector<vector<unsigned char>>>();			//  为记录所有i个碳的新结构开辟空间										
		int cc = 0;
		for (size_t j = 0; j < lastStructs->size(); j++) {		//  遍历i-1个碳的所有连接方式
			for (int k = 1; k <= i - 1; k++) {					//  遍历拓扑结构j上的所有碳原子
				if (lastStructs->at(j).at(k).size() > 3)		//  一个碳上最多连4个碳
					continue;
				curStruct.clear();
				curStruct = lastStructs->at(j);					//  记下原来的结构
				curStruct.resize(i + 1);
				curStruct.at(k).push_back(i);					//  把新原子连到k号碳
				curStruct.at(i).push_back(k);
				tmp = utreeFunc.getSeq(curStruct);				//  获取curStruct无根树的括号序列
				if (isomers.insert(tmp).second == true) {
					cc++;
					if (cc > 2000)
						goto L;
					isomerCounter.at(i)++;
					newStructs->push_back(curStruct);
					if (allowAdd)
						result.push_back(utreeFunc.getSMILES(curStruct));
				}
			}
		}
	L:;
		isomers.clear();										//  清掉i个碳的所有合法括号序列
		delete lastStructs;										//  归还旧结构占用的空间
		lastStructs = newStructs;								//  旧指针转移
		newStructs = nullptr;
	}
}


bool AlkaneTopo::setNumberOfCarbon(int n) {
	if (n < 3 || n > 2500)
		return false;
	numberOfCarbon = n;
	isomerCounter.resize(n + 1, 0);
	isomerCounter[1] = isomerCounter[2] = 1;
	return true;
}

int AlkaneTopo::getNumberOfCarbon() const{
	return numberOfCarbon;
}

const vector<vector<unsigned char>>* AlkaneTopo::getCurStruct() {
	return &curStruct;
}

void UnrootedTree::dfsGetSMILES(int x, int father) {
	allSeq[x] = "(C";
	for (size_t i = 0; i < utree->at(x).size(); i++) {
		int p = utree->at(x).at(i);
		if (p != father)
			dfsGetSMILES(p, x);
	}
	int now = 0;
	for (size_t i = 0; i < utree->at(x).size(); i++) {
		int p = utree->at(x).at(i);
		if (p != father)
			partSeq[now++] = allSeq[p];
	}
	sort(partSeq.begin(), partSeq.begin() + now);
	for (int i = 0; i < now; i++) {
		allSeq[x] += partSeq[i];
	}
	allSeq[x] += ")";
}

string UnrootedTree::getSMILES(vector<vector<unsigned char>>& _utree) {	//  获取括号序列
	utree = &_utree;
	n = utree->size();							//  比碳原子数多一个 0,1-n
	son1.resize(n, 0);			son2.resize(n, 0);
	partSeq.resize(n, "");		allSeq.resize(n, "");
	nodeOfMaxSubTree = -1;						//  最大子树的节点数
	dfsCollectSizeOfSon(1, 0);
	string finalSeq = "";
	for (int i = 1; i <= n; i++) {				//  处理两个重心
		if (son2[i] == nodeOfMaxSubTree) {		//  等于最大子树的节点数
			dfsGetSMILES(i, 0);
			if (allSeq[i] > finalSeq)
				finalSeq = allSeq[i];
		}
	}
	utree = nullptr;
	son1.clear();		son2.clear();
	partSeq.clear();	allSeq.clear();
	finalSeq.pop_back();
	finalSeq.erase(finalSeq.begin());
	return finalSeq;
}

//  生成整型数组中要求的所有碳原子数的同分异构体的SMILES结构式，存进字符串数组
inline void getTargetSMILES(vector<string>& result, vector<int>& targetCarbonNum) {
	int maxC = std::numeric_limits<int>::min(), minC = std::numeric_limits<int>::max(), tmp;
	//  目标数组去重
	sort(targetCarbonNum.begin(), targetCarbonNum.end());
	for (size_t i = 0; i < targetCarbonNum.size() - 1; i++) {
		tmp = targetCarbonNum.at(i);
		for (size_t j = i + 1; j < targetCarbonNum.size(); j++) {
			if (tmp == targetCarbonNum.at(j))
				targetCarbonNum.erase(targetCarbonNum.begin() + j);
			else {
				break;
			}
		}
	}
	for (size_t i = 0; i < targetCarbonNum.size(); i++) {
		maxC = targetCarbonNum.at(i) > maxC ? targetCarbonNum.at(i) : maxC;
		minC = targetCarbonNum.at(i) < minC ? targetCarbonNum.at(i) : minC;
	}
	result.clear();
	if (minC <= 0 || maxC >= 20) {
		cout << "目标碳原子数不合理，合法性检查的设置是[1,20]" << endl;
		return;
	}

}
