#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
class ScutIrac {
public:
	//  目录组织: "xxx/SCUT_IRAC/Couch", Couch目录下, 有Couch_Digit_195, Couch_GB1_188等子文件夹
	ScutIrac(const char* absolute_path);
	//~ScutIrac();
	static void loadLabel(const char* f_mylabel = "my_label.txt", const char* f_scutlabel = "scut_label.txt");
	vector<vector<vector<pair<int, int>>>> imgs;
	vector<int> intLabels;
	vector<pair<int, int>> rects;
	vector<string> mylabels, scutlabels;
private:
	static vector<string> myLabel;
	static vector<vector<string>> scutLabel;
	int IsValidLabel(const char* label);
	void rawData2Img_Label(const string& filename);
	static map<string, int> s2iMap;
};

