#include "scutirac.h"
#include <sstream>
#include <cassert>

using namespace std;
using namespace std::filesystem;
ScutIrac::ScutIrac(const char* absolute_path) {
	if (scutLabel.empty()) {
		cerr << "在构造函数之前需调用ScutIrac::loadLabel()" << endl;
		exit(-1);
	}
	path couch_path(absolute_path);
	if (!exists(couch_path)) {
		cerr << "构造函数应输入形如\"xxx/SCUT_IRAC/Couch\"的数据集绝对路径" << endl;
		cerr << "检查是不是路径不存在？" << endl;
		exit(-1);
	}
	directory_entry entry(couch_path);
	if (entry.status().type() != file_type::directory) {
		cerr << "构造函数应输入形如\"xxx/SCUT_IRAC/Couch\"的数据集绝对路径" << endl;
		cerr << "检查是不是实际输入了文件名？" << endl;
		exit(-1);
	}
	directory_iterator subDirList(couch_path);
	for (auto& i : subDirList) {
		if (directory_entry(i.path()).status().type() != file_type::directory) {
			cout << "跳过：" << i.path().filename() << endl;
			continue;
		}
		cout << "开始处理子目录数据：" << i.path().filename() << endl;
		directory_iterator datList(i.path());
		for (auto& j : datList) {
			rawData2Img_Label(j.path().string());
		}
		cout << "*********************" << endl;
	}
	assert(labels.size() == imgs.size());
}

void ScutIrac::rawData2Img_Label(const string& filename) {
	if (filename.find(".idx") == string::npos) {
		return;
	}
	ifstream ifsm(filename, ios::in | ios::binary);
	if (!ifsm.is_open()) {
		cout << "打开文件失败：" << filename << endl;
		return;
	}
	//cout << "处理文件：" << filename << endl;
	const int LENOFWHITESPACE = 1024;
	long sampleSum = 0;						//4
	unsigned char SampleState = 0;			//1
	int OswIndex = 0;						//4
	int IdxIndex = 0;						//4
	long DatOffset = 0;						//4

	unsigned char WordLength;				//1
	unsigned char WordCode[LENOFWHITESPACE];//1*WordLength
	unsigned short int PointNum;			//2
	unsigned short int LineNum;				//2
	unsigned short int GetTimePointNum;		//2
	unsigned short int
		GetTimePointIndex[LENOFWHITESPACE];	//2*GetTimePointNum
	int ElapsedTime[LENOFWHITESPACE];		//4*GetTimePointNum
	unsigned short int StrokePointNum;		//2
	unsigned short int x, y;				//2+2
	int xmin, ymin, xmax, ymax;

	ifsm.read((char*)&sampleSum, 4);
	string dataname = "";
	int tmp = 0;
	while (filename[tmp] != '.')
		dataname += filename[tmp++];
	dataname += ".dat";
	ifstream dataifsm(dataname, ios::in | ios::binary);
	while (ifsm.read((char*)&SampleState, 1)) {
		ifsm.read((char*)&OswIndex, 4);
		ifsm.read((char*)&IdxIndex, 4);
		ifsm.read((char*)&DatOffset, 4);
		dataifsm.seekg(DatOffset, ios::beg);
		dataifsm.read((char*)&WordLength, 1);
		dataifsm.read((char*)WordCode, 1 * (int)WordLength);
		WordCode[2] = 0;
		//cout << (char*)WordCode << endl;
		//system("pause");
		dataifsm.read((char*)&PointNum, 2);
		dataifsm.read((char*)&LineNum, 2);
		dataifsm.read((char*)&GetTimePointNum, 2);
		dataifsm.read((char*)GetTimePointIndex, 2 * (int)GetTimePointNum);
		dataifsm.read((char*)ElapsedTime, 4 * (int)GetTimePointNum);

		if ((char*)WordCode == string("￥") && OswIndex == 3) {
			;//handle bug here
		}
		imgs.push_back(vector<vector<pair<int, int>>>());
		//labels.push_back(pair<string, pair<int, int>>());

		xmin = std::numeric_limits<int>::max();
		ymin = std::numeric_limits<int>::max();
		xmax = std::numeric_limits<int>::min();
		ymax = std::numeric_limits<int>::min();
		for (int i = 0; i < (int)LineNum; i++) {
			dataifsm.read((char*)&StrokePointNum, 2);
			imgs.back().push_back(vector<pair<int, int>>());
			for (int j = 0; j < (int)StrokePointNum; j++) {
				dataifsm.read((char*)&x, 2);
				dataifsm.read((char*)&y, 2);
				xmin = xmin < x ? xmin : x;
				ymin = ymin < y ? ymin : y;
				xmax = xmax > x ? xmax : x;
				ymax = ymax > y ? ymax : y;
				//cout << "x=" << x << "y=" << y << endl;
				//system("pause");
				imgs.back().back().push_back(pair<int, int>(x, y));
			}
		}

		int index = IsValidLabel((char*)WordCode);
		if (-1 != index) {
			rects.push_back(pair<int, int>(xmax - xmin, ymax - ymin));
			scutlabels.push_back((char*)WordCode);
			mylabels.push_back(myLabel.at(index));
			intLabels.push_back(s2iMap[mylabels.back()]);
			//移动到坐标原点
			for (auto& i : imgs.back()) {
				for (auto& j : i) {
					j = pair<int, int>(j.first - xmin, j.second - ymin);
					//cout << "x=" << j.first << "y=" << j.second << endl;
					//system("pause");
				}
			}
		}
		else {
			imgs.pop_back();
		}

	}
	dataifsm.close();
	ifsm.close();
}
map<string, int> ScutIrac::s2iMap;
vector<string> ScutIrac::myLabel;
vector<vector<string>> ScutIrac::scutLabel;

void ScutIrac::loadLabel(const char* f_mylabel, const char* f_scutlabel) {
	ifstream ifsm2(f_mylabel, ios::in), ifsm1(f_scutlabel, ios::in);
	if (!ifsm1.is_open() || !ifsm2.is_open()) {
		cerr << "文件打开失败：" << f_mylabel << " or " << f_scutlabel << endl;
		exit(-1);
	}
	string tmp, word;
	scutLabel.clear(); myLabel.clear();
	while (getline(ifsm1, tmp)) {
		if (tmp.size() > 0) {
			scutLabel.emplace_back(vector<string>());
			stringstream ss(tmp);
			while (ss >> word) {
				if (word.size() > 0) {
					scutLabel.back().emplace_back(word);
				}
			}
		}
	}
	int index = 0;
	while (getline(ifsm2, tmp)) {
		if (tmp.size() > 0) {
			myLabel.emplace_back(tmp);
			s2iMap[tmp] = index;
			index++;
		}
	}
	//for (auto& i : scutLabel) {
	//	for (auto& j : i) {
	//		cout << "-" << j << "-";
	//	}
	//	cout << endl;
	//}
	//for (auto& i : myLabel) {
	//	cout << "-" << i << "-" << endl;
	//}
	if (scutLabel.size() != myLabel.size()) {
		cerr << "标签文件错误：" << scutLabel.size() << "," << myLabel.size() << endl;
		exit(-1);
	}
}


int ScutIrac::IsValidLabel(const char* label) {
	for (size_t i = 0; i < scutLabel.size(); i++) {
		for (auto& j : scutLabel.at(i)) {
			if (j == label)
				return i;
		}
	}
	return -1;
}
