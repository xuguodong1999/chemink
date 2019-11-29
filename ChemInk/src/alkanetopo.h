#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include <set>

class UnrootedTree {
private:
	std::vector<std::vector<unsigned char>>* utree;
	std::vector<std::string> partSeq, allSeq;
	std::vector<int> son1, son2;
	int n;
	int nodeOfMaxSubTree;
	void dfsCollectSizeOfSon(int x, int father);
	void dfsGetSeq(int x, int father);
	void dfsGetSMILES(int x, int father);
public:
	UnrootedTree();
	~UnrootedTree();
	std::string getSeq(std::vector<std::vector<unsigned char>>& _utree);
	//  获取按照SMILES格式表示的分子式
	std::string getSMILES(std::vector<std::vector<unsigned char>>& _utree);
	std::vector<int>* getSubTreeSize(std::vector<std::vector<unsigned char>>& _utree);
	void clearSubTreeSize();
	void display();
	int getMaxSubTree();
};

class AlkaneTopo {
private:
	std::vector<std::vector<std::vector<unsigned char>>>* lastStructs;		//  i-1个碳的所有连接方式
	std::vector<std::vector<std::vector<unsigned char>>>* newStructs;		//  i个碳的所有连接方式
	std::vector<std::vector<unsigned char>> curStruct;				//  当前被拿出来比较的一个结构 使用邻接表表示无根树
	//map<std::string, int> isomers;								//  所有括号序列 map<括号序,索引号>
	//unordered_set<std::string> isomers;
	std::set<std::string> isomers;
	std::vector<int> isomerCounter;
	int numberOfCarbon;
	UnrootedTree utreeFunc;

public:
	AlkaneTopo();
	~AlkaneTopo();
	void getAlkaneTopo();
	void getAlkaneTopo(std::vector<std::string>& result, std::vector<int>& targetCarbonNum);
	bool setNumberOfCarbon(int n);									//  设置并检查碳原子数
	int getNumberOfCarbon()const;
	const std::vector<std::vector<unsigned char>>* getCurStruct();			//  返回表示当前拓扑的邻接表地址
};

inline void getTargetSMILES(std::vector<std::string>& result, std::vector<int>& targetCarbonNum);
