#include "spellcorrector.h"
#include <iostream>
#include <QFile>
using namespace std;

//LETTERTYPE getType(vector<LETTERTYPE> _env) {
//	switch (_env.size()) {
//	case 0:
//		return LETTERTYPE::NONE;
//	case 1:
//		break;
//	case 2:
//		break;
//	default:
//		break;
//	}
//	switch (_index){
//	case 0:
//		return LETTERTYPE::DIGIT; break;		//  数字
//	case 0:
//		return LETTERTYPE::CAPITAL; break;	//  大写字母
//	case 0:
//		return LETTERTYPE::LITTLE; break;		//  小写字母
//	case 0:
//		return LETTERTYPE::GREEK; break;		//  希腊字母
//	case 0:
//		return LETTERTYPE::MATH; break;		//  数学运算符
//	case 0:
//		return LETTERTYPE::INFO; break;		//  注释符如上下箭头
//	case 0:
//		return LETTERTYPE::SYMBOL; break;		//  标点符号
//	case 0:
//		return LETTERTYPE::NONE; break;		//  无意义字符
//	default:
//		break;
//	}
//	return LETTERTYPE::NONE;
//}
//
////  similarWords[...]={x1,x2,...xn};
////  handle x1,...,xn
//vector<string> dim2Precise(vector<vector<int>>& _inputWords) {
//	vector<string> results;
//	//  先索引模糊表，得到模糊表大小，局部遍历模糊表，获得字符类型，送入决策树
//	for (auto& probs : _inputWords) {
//		;
//	}
//
//
//	for (auto& i : similarWords) {
//		for (auto& j : i) {
//			cout << j << endl;
//		}
//	}
//	return results;
//}

vector<string> SpellCorrector::validWords;

SpellCorrector::SpellCorrector() {
	if (validWords.empty()) {
		QFile freader(":/Resources/language/dictionary.txt");
		freader.open(QIODevice::ReadOnly | QIODevice::Text);
		char tmp[50];
		if (freader.isOpen()) {
			freader.readLine(tmp, 50);
			while (true) {
				validWords.push_back(tmp);
				validWords.back().pop_back();
				freader.readLine(tmp, 50);
				if (freader.atEnd())
					break;
			}
		}
		else {
			std::cout << "无法打开字典文件" << std::endl;
		}
		freader.close();
	}
}

SpellCorrector::~SpellCorrector() {

}

int SpellCorrector::getCnnIndex(int outIndex) {
	switch (outIndex) {
	case  0:case  26:// 3 和 (ζ ξ)
		return 49;
	case  1:
		return 50;
	case  2:
		return 51;
	case  3:
		return 75;
	case  4:
		return 52;
	case  5:
		return 21;
	case  6:
		return 53;
	case  7:
		return 18;
	case  8:
		return 54;
	case  9:
		return 55;
	case  10:case 31:// #和H
		return 56;
	case  11:
		return 57;
	case  12:
		return 58;
	case  13:
		return 59;
	case  14:
		return 60;
	case  15:
		return 61;
	case  16:
		return 19;
	case  17:
		return 14;
	case  18:
		return 29;
	case  19:
		return 5;
	case  20:
		return -1;
	case  21:
		return 4;
	case  22:
		return 62;
	case  23:
		return 34;
	case  24:
		return 42;
	case  25:
		return 63;
		//case  26:
		//	return 20;
	case  27:
		return 26;
	case  28:
		return 71;
	case  29:
		return 17;
	case  30:
		return 64;
		//case  31:
		//	return 36;
	case  32:
		return 65;
	case  33:
		return 66;
	case  34:
		return 67;
	case  35:
		return 7;
	case  36:
		return 0;
	case  37:
		return 72;
	case  38:
		return 45;
	case  39:
		return 40;
	case  40:
		return 23;
	case  41:
		return 25;
	case  42:
		return 43;
	case  43:
		return 47;
	case  44:
		return 13;
	case  45:
		return 24;
	case  46:
		return 33;
	case  47:
		return 38;
	case  48:
		return 27;
	case  49:
		return 35;
	case  50:
		return 30;
	case  51:
		return 68;
	case  52:
		return 3;
	case  53:
		return 74;
	case  54:
		return 8;
	case  55:
		return 15;
	case  56:
		return 32;
	case  57:
		return 6;
	case  58:
		return 46;
	case  59:
		return 39;
	case  60:
		return 37;
	case  61:
		return 69;
	case  62:
		return 44;
	case  63:
		return 48;
	case  64:
		return 2;
	case  65:
		return 14;
	case  66:
		return 22;
	case  67:
		return 70;
	case  68:
		return 9;
	case  69:
		return 41;
	case  70:
		return 12;
	case  71:
		return 10;
	case  72:
		return 28;
	case  73:
		return -2;
	case  74:
		return 11;
	case  75:
		return 31;
	case  76:
		return 16;
	case  77:
		return 1;
	default:
		return -3;
	}
}

int SpellCorrector::getYoloIndex(int outIndex) {
	switch (outIndex) {
	default:
		return outIndex;
	}
}

string SpellCorrector::getChemAscii(const std::string& _src) {
	map<string, string>::iterator iter = unicode2chemAsciiMap.find(_src);
	if (iter != unicode2chemAsciiMap.end()) {
		return iter->second;
	}
	else {
		return "[]";
	}
}

map<std::string, std::string> SpellCorrector::unicode2chemAsciiMap = {
	{"÷","/"},
	{"≠","=/="},
	{"≤","<="},
	{"≥",">="},
	{"＄","$"},
	{"→","->"},
	{"←","<-"},
	{"↑","(g)"},
	{"↓","(s)"},
	{"＃","#"},
	{"￥","RMB"},
	{"％","%"},
	{"＆","&"},
	{"（","("},
	{"）",")"},
	{"＊","*"},
	{"＋","+"},
	{"－","-"},
	{"０","0"},
	{"１","1"},
	{"２","2"},
	{"３","3"},
	{"４","4"},
	{"５","5"},
	{"６","6"},
	{"７","7"},
	{"８","8"},
	{"９","9"},
	{"＜","<"},
	{"＝","="},
	{"＞",">"},
	{"？","?"},
	{"＠","@"},
	{"Ａ","A"},
	{"Ｂ","B"},
	{"Ｃ","C"},
	{"Ｄ","D"},
	{"Ｅ","E"},
	{"Ｆ","F"},
	{"Ｇ","G"},
	{"Ｈ","H"},
	{"Ｉ","I"},
	{"Ｊ","J"},
	{"Ｋ","K"},
	{"Ｌ","L"},
	{"Ｍ","M"},
	{"Ｎ","N"},
	{"Ｏ","O"},
	{"Ｐ","P"},
	{"Ｑ","Q"},
	{"Ｒ","R"},
	{"Ｓ","S"},
	{"Ｔ","T"},
	{"Ｕ","U"},
	{"Ｖ","V"},
	{"Ｗ","W"},
	{"Ｘ","X"},
	{"Ｙ","Y"},
	{"Ｚ","Z"},
	{"［","["},
	{"］","]"},
	{"ａ","a"},
	{"ｂ","b"},
	{"ｃ","c"},
	{"ｄ","d"},
	{"ｅ","e"},
	{"ｆ","f"},
	{"ｇ","g"},
	{"ｈ","h"},
	{"ｉ","i"},
	{"ｊ","j"},
	{"ｋ","k"},
	{"ｌ","l"},
	{"ｍ","m"},
	{"ｎ","n"},
	{"ｏ","o"},
	{"ｐ","p"},
	{"ｑ","q"},
	{"ｒ","r"},
	{"ｓ","s"},
	{"ｔ","t"},
	{"ｕ","u"},
	{"ｖ","v"},
	{"ｗ","w"},
	{"ｘ","x"},
	{"ｙ","y"},
	{"ｚ","z"},
	{"｛","{"},
	{"｝","}"},
	{"Π","..."},
	{"Σ","..."},
	{"Φ","..."},
	{"Ψ","..."},
	{"Ω","..."},
	{"α","alpha"},
	{"β","beta"},
	{"γ","..."},
	{"δ","delta"},
	{"ε","..."},
	{"ζ","..."},
	{"η","..."},
	{"θ","..."},
	{"λ","lambda"},
	{"μ","..."},
	{"ξ","..."},
	{"π","..."},
	{"ρ","..."},
	{"σ","sigma"},
	{"τ","..."},
	{"φ","..."},
	{"ψ","..."},
	{"ω","..."},
	{"⊕","nor"}
};

const vector<vector<std::string>> SpellCorrector::similarWords = {
	//  复杂部分 不做要求
	{"１","Ｉ","ｌ"},
	{"２","Ｚ","ｚ" },
	{"５","Ｓ","ｓ"},
	{"０","Ｏ","ｏ"},
	{"Ｃ","ｃ"},
	{"６","ｂ"},
	{"９","ｇ","ｑ"},
	{"Ｋ","ｋ"},
	{"ρ","Ｐ","ｐ"},
	{"μ","Ｕ","ｕ"},
	{"ω","Ｗ","ｗ"},
	{"Ｘ","ｘ"},
	{"Ｖ","ｖ"},
	{"Ｍ","ｍ"},
	{"σ","α","ａ"},
	{"π","Π"},
	{"γ","Ｙ","ｙ"},
	{"Φ","Ψ","φ","ψ"},
	{"ε","Ｅ"},
	{"τ","Ｔ"},
	{"ζ","ξ"},
	{"Ｂ","β","＄","＆"},
	//  简单部分
	{"θ"}, { "≤" }, { "≥" }, { "＜" }, { "＝" }, { "＞" }, { "？" },
	{"＠"},{"≠"},{"⊕"},{"＋"},{"－"},{"÷"},{"＊"},{"＃"},{"￥"},{"％"},
	{"→"},{"←"},{"↑"},{"↓"},{"（"},{"）"},
	{"｛"},{"｝"},{"［"},{"］"},{"３"},{"４"},{"７"},{"Ａ"}, {"Ｄ"},{"Ｆ"},{"Ｇ"},
	{"Ｈ"},{"Ｊ"},{"Ｌ"},{"Ｎ"},{"Ｑ"},{"Ｒ"},{"ｄ"},{"ｅ"},{"ｆ"},{"ｈ"},{"ｉ"},
	{"ｊ"},{"ｎ"},{"ｒ"},{"ｔ"},{"η"},{"λ"},{"Σ"},{"Ω"},{"δ","８"}
};
/*（*\信息标记）（数字）（*\小写字母）
（*\信息标记）（小写字母）（）
（*\信息标记）（大写字母）（）
（*\分隔符）（分隔符）（）
（数字、小写字母、大写字母）（信息标记）（分隔符）
3、人工设定的分拣规则
形式：（xxx -> x）（xxx : x）（xxx->x）*/

// 人工分拣纯属扯淡，我计划先用几条简单的规则把程序跑起来
// 语言模型之前没接触过，我也没时间做脚本训练和c++环境部署了
// 凑合着做吧

vector<string> SpellCorrector::getResult(const vector<vector<string>>& _src) {
	vector<string> result;
	//  _src包含基本ascii字符
	if (_src.size() == 1) {
		for (auto& i : _src.at(0)) {
			int tmp = getWordType(i);
			if (tmp == DIGIT || tmp == CAPITAL || tmp == LITTLE) {
				result.push_back(i);
				return result;
			}
		}
	}
	//  正向解析
	for (size_t i = 1; i < _src.size(); i++) {
		for (auto& j : _src.at(i)) {
			switch (getWordType(result.at(i - 1))) {
			case DIGIT:
				break;
			case LITTLE:
				break;
			case CAPITAL:
				break;
			case INFO:
				break;
			case EXPLICTCUT:
				break;
			case LB:
				break;
			case RB:
				break;
			default:
				break;
			}
		}
	}
	//  反向解析

	//  双向解析
	return result;
}

//map<string, int> SpellCorrector::chemAscii2typeMap = { };
int SpellCorrector::getWordType(const std::string& _word) {
	if (_word.length() == 1) {
		if (_word.at(0) >= '0' && _word.at(0) <= '9') {
			return DIGIT;
		}
		else if (_word.at(0) >= 'A' && _word.at(0) <= 'Z') {
			return CAPITAL;
		}
		else if (_word.at(0) >= 'a' && _word.at(0) <= 'z') {
			return LITTLE;
		}
		else if (_word.at(0) == '(' || _word.at(0) == '{' || _word.at(0) == '[') {
			return LB;
		}
		else if (_word.at(0) == ')' || _word.at(0) == '}' || _word.at(0) == ']') {
			return RB;
		}
		else if (_word.at(0) == '#' || _word.at(0) == '=' || _word.at(0) == '+'
			|| _word.at(0) == '-' || _word.at(0) == '>' || _word.at(0) == '<') {
			return EXPLICTCUT;
		}
	}
	else if (_word.length() == 2) {
		if (_word == "->" || _word == "<-" || _word == "<=" || _word == ">=") {
			return EXPLICTCUT;
		}
	}
	else if (_word.length() == 3) {
		if (_word == "(g)" || _word == "(s)") {
			return INFO;
		}
	}
	return NONE;
}


string SpellCorrector::getValidWord(const vector<vector<string>>& _src,
	int start, int end) {
	vector<int> scores;
	int cmplen;
	int score, baseScore;
	const size_t targetlen = end - start + 1;
	for (size_t i = 0; i < validWords.size(); i++) {
		string& word = validWords.at(i);
		cmplen = std::min(word.length(), targetlen);
		score = 0;
		baseScore = 100;
		if (targetlen == cmplen) {
			score += baseScore;
		}
		for (int j = 0; j < cmplen; j++) {
			for (auto& k : _src.at(j + start)) {
				if (k.length() == 1 && k.at(0) == word.at(j)) {
					score += baseScore;
					break;
				}
			}
		}
		scores.push_back(score);
	}
	size_t index = 0;
	for (size_t i = 1; i < scores.size(); i++) {
		if (scores.at(index) < scores.at(i)) {
			index = i;
		}
	}
	cout << "validWords.at(index)=" << validWords.at(index) << endl;
	return validWords.at(index).substr(0, targetlen);
}

void SpellCorrector::getBestResult(const vector<vector<string>>& _src,
	vector<string>& result) {
	result.clear();
	vector<vector<string>> src;
	for (auto& i : _src) {
		src.push_back(vector<string>());
		for (auto& j : i) {
			src.back().push_back(getChemAscii(j));
		}
	}
	vector<int> divPos;
	divPos.push_back(-1);
	for (size_t i = 0; i < src.size(); i++) {
		bool isDiv(false);
		for (auto& j : src.at(i)) {
			if (getWordType(j) == EXPLICTCUT) {
				isDiv = true;
				break;
			}
		}
		if (isDiv) {
			divPos.push_back(i);
		}
	}
	divPos.push_back(src.size());
	cout << "divPos.size()=" << divPos.size() << endl;
	int start, end;
	for (size_t i = 1; i < divPos.size(); i++) {
		start = divPos.at(i - 1) + 1; end = divPos.at(i) - 1;
		if (start <= end) {
			result.push_back(
				getValidWord(src, start, end)
			);
		}
		if (i != divPos.size() - 1) {
			result.push_back(src.at(divPos.at(i)).at(0));
		}
	}
	/*for (auto& i : result) {
		cout << i << " ";
	}*/
}
