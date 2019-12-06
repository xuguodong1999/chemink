#include "stdafx.h"
#include "chembalancer.h"

//#define TESTCEI
//#define WINSYS

#ifdef WINSYS
#include"Windows.h"
#include"Psapi.h"
#endif

#include<iostream>
#include<iomanip>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<vector>
#include<algorithm>

using std::string;
using std::cout;
using std::endl;
using std::vector;
#define MAXM 50
#define ELECOL 3
#define MAXC 1000

//  声明
namespace chemfunction {
	class Fraction {
	public:
		Fraction();
		Fraction(int x);
		~Fraction();
		Fraction operator=(int x);
		Fraction operator=(const Fraction x);
		Fraction operator+(const Fraction x);
		Fraction operator-(const Fraction x);
		Fraction operator*(const Fraction x);
		Fraction operator/(const Fraction x);
		bool operator<(const Fraction x);
		bool operator<=(const Fraction x);
		bool operator>(const Fraction x);
		bool operator>=(const Fraction x);
		bool operator==(const Fraction x);
		bool operator!=(const Fraction x);
		void a2f(char* tar);
		void a2f(string& tar);
		string p();
		friend Fraction s2Frac(char* tar);
		friend Fraction s2Frac(const string& tar);
		friend Fraction Abs(Fraction x);
		friend bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]);
		friend void Matrix_row_transformation(int m[][MAXM], int& row, int& col);

		int a;
		int b; //  a / b
		void reduce();
	};
	Fraction s2Frac(char* tar);
	Fraction s2Frac(const string& tar);
	Fraction Abs(Fraction x);


	int gcd(int x, int y);
	int lcm(int x, int y);
	bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]);
	void Matrix_row_transformation(int m[][MAXM], int& row, int& col);



	//整型数转字符串
	char* i2a(int num, char* str, int radix);

	//从字符串s中删除所有字符串mark
	void deleteAllMark(string& s, const string& mark);

	//把字符串str中的所有old_value替换为new_value
	void replaceAllMark(string& str, const string& old_value, const string& new_value);

	//为字符串tar按照化学方程式书写规则添加HTML标签
	string addHtml(string& tar);

	//返回元素ele的质子数
	int Get_proton_num(string& ele);

	//返回元素ele的相对原子质量
	double Get_relative_atomic_mass(string& ele);

	//返回第pn号元素的相对原子质量
	double Get_relative_atomic_mass(int proton_num);

	//返回第pn号元素的电负性
	double Get_electronegativity(int proton_num);

	//打印当前内存（Windows操作系统下）
	void printMemory();

	class Element {
	public:
		Element();
		Element(string& _name);
		~Element();
		string name;
		int amount;
		Fraction valence;

		void clear();
		bool Exist();

		bool Reduced;
		Fraction finalValence;
	};

	bool namelessthan(const Element& a, const Element& b);

	class MatNode {
	public:
		bool isNum;
		bool isHead; //  用于建立语法树以及下标显示 控制台程序用不到
		int num;
		vector<Element> block; //  block 代表元素阵列 记录一个matnode里面的所有元素
		MatNode();
		~MatNode();
		void clear();
		void compress(); //  压缩元素阵列 合并相同的元素 累加元素数量
		void print();
	};

	class Matter {
	public:
		Matter();
		Matter(string& _name);
		Matter(char* _name);
		~Matter();

		string name;
		int amount;
		int q;
		char state;
		string stateInfo;

		void clear();
		void inputName();
		bool Exist();
		void getStateInfo();

		vector<Element> eleList;
		bool intoElement();
		void printElement();
		bool getValence();
		void printValence();

		int getEleAmount(string& _element);
		int getEleIndex(string& _element);
		string p();

		double getRelativeMolecureMass();

	private:
		bool preProcess(string& tar);
		bool CallastBlock(vector<MatNode>& matlist);
		bool CalaBlock(vector<MatNode>& matlist, int i, int j);
		bool lexicalAnalysis(vector<MatNode>& matlist, string& tar);
		void furtherLexicalAnalysis(vector<MatNode>& matlist);
	};

	class Equation {
	public:
		Equation();
		~Equation();
		string content;
		void clear();
		void inputContent();
		string p();
		bool IsConversed(vector<int>& raw);
		vector<int> oRatioList;
		vector<int> ratioList;
		bool exec(string& _content);
		bool exec();
		int* pIndex(int numberOfMolecule);

	private:
		string equation;
		bool ie;
		vector<Matter> mRList;
		vector<Matter> mPList;
		vector<Element> eRList;
		vector<Element> ePList;
		vector<Fraction> redox;
		bool checkFormat();
		void split();
		bool intoMatter();
		bool intoElement();

		void getState();
		void addStateInfo();

		bool balance();
		bool getRedox();
		int getEleIndex(string& tarelenam);
	};
};
//  定义
namespace chemfunction {
	char* i2a(int num, char* str, int radix) {
		char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		unsigned unum;
		int i = 0, j, k;
		if (radix == 10 && num < 0) {
			unum = (unsigned)-num;
			str[i++] = '-';
		}
		else
			unum = (unsigned)num;
		do {
			str[i++] = index[unum % (unsigned)radix];
			unum /= radix;
		} while (unum);
		str[i] = '\0';
		if (str[0] == '-')
			k = 1;
		else
			k = 0;
		char temp;
		for (j = k; j <= (i - k - 1) / 2.0; j++) {
			temp = str[j];
			str[j] = str[i - j - 1];
			str[i - j - 1] = temp;
		}
		return str;
	}

	void deleteAllMark(string& s, const string& mark) {
		size_t nSize = mark.size();
		size_t pos = 0;
		while (1) {
			pos = s.find(mark);
			if (pos == string::npos)
				return;
			s.erase(pos, nSize);
		}
	}

	void replaceAllMark(string& str, const string& old_value, const string& new_value) {
		for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
			if ((pos = str.find(old_value, pos)) != string::npos)
				str.replace(pos, old_value.length(), new_value);
			else
				break;
		}
	}

	int Get_proton_num(string& ele) {
		if (ele == "H")return 1;
		else if (ele == "He")return 2;
		else if (ele == "Li")return 3;
		else if (ele == "Be")return 4;
		else if (ele == "B")return 5;
		else if (ele == "C")return 6;
		else if (ele == "N")return 7;
		else if (ele == "O")return 8;
		else if (ele == "F")return 9;
		else if (ele == "Ne")return 10;
		else if (ele == "Na")return 11;
		else if (ele == "Mg")return 12;
		else if (ele == "Al")return 13;
		else if (ele == "Si")return 14;
		else if (ele == "P")return 15;
		else if (ele == "S")return 16;
		else if (ele == "Cl")return 17;
		else if (ele == "Ar")return 18;
		else if (ele == "K")return 19;
		else if (ele == "Ca")return 20;
		else if (ele == "Sc")return 21;
		else if (ele == "Ti")return 22;
		else if (ele == "V")return 23;
		else if (ele == "Cr")return 24;
		else if (ele == "Mn")return 25;
		else if (ele == "Fe")return 26;
		else if (ele == "Co")return 27;
		else if (ele == "Ni")return 28;
		else if (ele == "Cu")return 29;
		else if (ele == "Zn")return 30;
		else if (ele == "Ga")return 31;
		else if (ele == "Ge")return 32;
		else if (ele == "As")return 33;
		else if (ele == "Se")return 34;
		else if (ele == "Br")return 35;
		else if (ele == "Kr")return 36;
		else if (ele == "Rb")return 37;
		else if (ele == "Sr")return 38;
		else if (ele == "Y")return 39;
		else if (ele == "Zr")return 40;
		else if (ele == "Nb")return 41;
		else if (ele == "Mo")return 42;
		else if (ele == "Tc")return 43;
		else if (ele == "Ru")return 44;
		else if (ele == "Rh")return 45;
		else if (ele == "Pd")return 46;
		else if (ele == "Ag")return 47;
		else if (ele == "Cd")return 48;
		else if (ele == "In")return 49;
		else if (ele == "Sn")return 50;
		else if (ele == "Sb")return 51;
		else if (ele == "Te")return 52;
		else if (ele == "I")return 53;
		else if (ele == "Xe")return 54;
		else if (ele == "Cs")return 55;
		else if (ele == "Ba")return 56;
		else if (ele == "La")return 57;
		else if (ele == "Ce")return 58;
		else if (ele == "Pr")return 59;
		else if (ele == "Nd")return 60;
		else if (ele == "Pm")return 61;
		else if (ele == "Sm")return 62;
		else if (ele == "Eu")return 63;
		else if (ele == "Gd")return 64;
		else if (ele == "Tb")return 65;
		else if (ele == "Dy")return 66;
		else if (ele == "Ho")return 67;
		else if (ele == "Er")return 68;
		else if (ele == "Tm")return 69;
		else if (ele == "Yb")return 70;
		else if (ele == "Lu")return 71;
		else if (ele == "Hf")return 72;
		else if (ele == "Ta")return 73;
		else if (ele == "W")return 74;
		else if (ele == "Re")return 75;
		else if (ele == "Os")return 76;
		else if (ele == "Ir")return 77;
		else if (ele == "Pt")return 78;
		else if (ele == "Au")return 79;
		else if (ele == "Hg")return 80;
		else if (ele == "Tl")return 81;
		else if (ele == "Pb")return 82;
		else if (ele == "Bi")return 83;
		else if (ele == "Po")return 84;
		else if (ele == "At")return 85;
		else if (ele == "Rn")return 86;
		else if (ele == "Fr")return 87;
		else if (ele == "Ra")return 88;
		else if (ele == "Ac")return 89;
		else if (ele == "Th")return 90;
		else if (ele == "Pa")return 91;
		else if (ele == "U")return 92;
		else if (ele == "Np")return 93;
		else if (ele == "Pu")return 94;
		else if (ele == "Am")return 95;
		else if (ele == "Cm")return 96;
		else if (ele == "Bk")return 97;
		else if (ele == "Cf")return 98;
		else if (ele == "Es")return 99;
		else if (ele == "Fm")return 100;
		else if (ele == "Md")return 101;
		else if (ele == "No")return 102;
		else if (ele == "Lr")return 103;
		else if (ele == "Rf")return 104;
		else if (ele == "Db")return 105;
		else if (ele == "Sg")return 106;
		else if (ele == "Bh")return 107;
		else if (ele == "Hs")return 108;
		else if (ele == "Mt")return 109;
		else if (ele == "Ds")return 110;
		else if (ele == "Rg")return 111;
		else if (ele == "Cn")return 112;
		else return -1;
	}

	double Get_relative_atomic_mass(string& ele) {
		double relative_atomic_mass = 0;
		switch (Get_proton_num(ele)) {
		case 0:relative_atomic_mass = 0; break;
		case 1:relative_atomic_mass = 1.007947; break;
		case 2:relative_atomic_mass = 4.0026022; break;
		case 3:relative_atomic_mass = 6.9412; break;
		case 4:relative_atomic_mass = 9.0121823; break;
		case 5:relative_atomic_mass = 10.8117; break;
		case 6:relative_atomic_mass = 12.01078; break;
		case 7:relative_atomic_mass = 14.00672; break;
		case 8:relative_atomic_mass = 15.99943; break;
		case 9:relative_atomic_mass = 18.99840325; break;
		case 10:relative_atomic_mass = 20.17976; break;
		case 11:relative_atomic_mass = 22.989769282; break;
		case 12:relative_atomic_mass = 24.30506; break;
		case 13:relative_atomic_mass = 26.98153868; break;
		case 14:relative_atomic_mass = 28.08553; break;
		case 15:relative_atomic_mass = 30.9737622; break;
		case 16:relative_atomic_mass = 32.0655; break;
		case 17:relative_atomic_mass = 35.4532; break;
		case 18:relative_atomic_mass = 39.9481; break;
		case 19:relative_atomic_mass = 39.09831; break;
		case 20:relative_atomic_mass = 40.0784; break;
		case 21:relative_atomic_mass = 44.9559126; break;
		case 22:relative_atomic_mass = 47.8671; break;
		case 23:relative_atomic_mass = 50.94151; break;
		case 24:relative_atomic_mass = 51.99616; break;
		case 25:relative_atomic_mass = 54.9380455; break;
		case 26:relative_atomic_mass = 55.8452; break;
		case 27:relative_atomic_mass = 58.9331955; break;
		case 28:relative_atomic_mass = 58.69342; break;
		case 29:relative_atomic_mass = 63.5463; break;
		case 30:relative_atomic_mass = 65.394; break;
		case 31:relative_atomic_mass = 69.7231; break;
		case 32:relative_atomic_mass = 72.641; break;
		case 33:relative_atomic_mass = 74.921602; break;
		case 34:relative_atomic_mass = 78.963; break;
		case 35:relative_atomic_mass = 79.9041; break;
		case 36:relative_atomic_mass = 83.7982; break;
		case 37:relative_atomic_mass = 85.46783; break;
		case 38:relative_atomic_mass = 87.621; break;
		case 39:relative_atomic_mass = 88.905852; break;
		case 40:relative_atomic_mass = 91.2242; break;
		case 41:relative_atomic_mass = 92.906382; break;
		case 42:relative_atomic_mass = 95.942; break;
		case 43:relative_atomic_mass = 97.9072; break;
		case 44:relative_atomic_mass = 101.072; break;
		case 45:relative_atomic_mass = 102.905502; break;
		case 46:relative_atomic_mass = 106.421; break;
		case 47:relative_atomic_mass = 107.86822; break;
		case 48:relative_atomic_mass = 112.4118; break;
		case 49:relative_atomic_mass = 114.8183; break;
		case 50:relative_atomic_mass = 118.7107; break;
		case 51:relative_atomic_mass = 121.7601; break;
		case 52:relative_atomic_mass = 127.603; break;
		case 53:relative_atomic_mass = 126.904473; break;
		case 54:relative_atomic_mass = 131.2936; break;
		case 55:relative_atomic_mass = 132.90545192; break;
		case 56:relative_atomic_mass = 137.3277; break;
		case 57:relative_atomic_mass = 137.3277; break;
		case 58:relative_atomic_mass = 140.1161; break;
		case 59:relative_atomic_mass = 140.907652; break;
		case 60:relative_atomic_mass = 144.2423; break;
		case 61:relative_atomic_mass = 145; break;
		case 62:relative_atomic_mass = 150.362; break;
		case 63:relative_atomic_mass = 151.9641; break;
		case 64:relative_atomic_mass = 157.253; break;
		case 65:relative_atomic_mass = 158.925352; break;
		case 66:relative_atomic_mass = 162.5001; break;
		case 67:relative_atomic_mass = 164.930322; break;
		case 68:relative_atomic_mass = 167.2593; break;
		case 69:relative_atomic_mass = 168.934212; break;
		case 70:relative_atomic_mass = 173.043; break;
		case 71:relative_atomic_mass = 174.9671; break;
		case 72:relative_atomic_mass = 178.492; break;
		case 73:relative_atomic_mass = 180.947882; break;
		case 74:relative_atomic_mass = 183.841; break;
		case 75:relative_atomic_mass = 186.2071; break;
		case 76:relative_atomic_mass = 190.233; break;
		case 77:relative_atomic_mass = 192.2173; break;
		case 78:relative_atomic_mass = 195.0849; break;
		case 79:relative_atomic_mass = 196.9665694; break;
		case 80:relative_atomic_mass = 200.592; break;
		case 81:relative_atomic_mass = 204.38332; break;
		case 82:relative_atomic_mass = 207.21; break;
		case 83:relative_atomic_mass = 208.980401; break;
		case 84:relative_atomic_mass = 208.9824; break;
		case 85:relative_atomic_mass = 209.9871; break;
		case 86:relative_atomic_mass = 222.0176; break;
		case 87:relative_atomic_mass = 223; break;
		case 88:relative_atomic_mass = 226; break;
		case 89:relative_atomic_mass = 227; break;
		case 90:relative_atomic_mass = 232.038062; break;
		case 91:relative_atomic_mass = 231.035882; break;
		case 92:relative_atomic_mass = 238.028913; break;
		case 93:relative_atomic_mass = 238.8486; break;
		case 94:relative_atomic_mass = 242.8798; break;
		case 95:relative_atomic_mass = 244.8594; break;
		case 96:relative_atomic_mass = 246.911; break;
		case 97:relative_atomic_mass = 248.9266; break;
		case 98:relative_atomic_mass = 252.9578; break;
		case 99:relative_atomic_mass = 253.9656; break;
		case 100:relative_atomic_mass = 259.0046; break;
		case 101:relative_atomic_mass = 260.0124; break;
		case 102:relative_atomic_mass = 261.0202; break;
		case 103:relative_atomic_mass = 264.0436; break;
		case 104:relative_atomic_mass = 269.0826; break;
		case 105:relative_atomic_mass = 270.0904; break;
		case 106:relative_atomic_mass = 273.1138; break;
		case 107:relative_atomic_mass = 274.1216; break;
		case 108:relative_atomic_mass = 272.106; break;
		case 109:relative_atomic_mass = 278.1528; break;
		case 110:relative_atomic_mass = 283.1918; break;
		case 111:relative_atomic_mass = 282.184; break;
		case 112:relative_atomic_mass = 287.223; break;
		default:relative_atomic_mass = -1; break;
		}
		return relative_atomic_mass;
	}

	double Get_relative_atomic_mass(int proton_num) {
		double relative_atomic_mass = 0;
		switch (proton_num) {
		case 0:relative_atomic_mass = 0; break;
		case 1:relative_atomic_mass = 1.007947; break;
		case 2:relative_atomic_mass = 4.0026022; break;
		case 3:relative_atomic_mass = 6.9412; break;
		case 4:relative_atomic_mass = 9.0121823; break;
		case 5:relative_atomic_mass = 10.8117; break;
		case 6:relative_atomic_mass = 12.01078; break;
		case 7:relative_atomic_mass = 14.00672; break;
		case 8:relative_atomic_mass = 15.99943; break;
		case 9:relative_atomic_mass = 18.99840325; break;
		case 10:relative_atomic_mass = 20.17976; break;
		case 11:relative_atomic_mass = 22.989769282; break;
		case 12:relative_atomic_mass = 24.30506; break;
		case 13:relative_atomic_mass = 26.98153868; break;
		case 14:relative_atomic_mass = 28.08553; break;
		case 15:relative_atomic_mass = 30.9737622; break;
		case 16:relative_atomic_mass = 32.0655; break;
		case 17:relative_atomic_mass = 35.4532; break;
		case 18:relative_atomic_mass = 39.9481; break;
		case 19:relative_atomic_mass = 39.09831; break;
		case 20:relative_atomic_mass = 40.0784; break;
		case 21:relative_atomic_mass = 44.9559126; break;
		case 22:relative_atomic_mass = 47.8671; break;
		case 23:relative_atomic_mass = 50.94151; break;
		case 24:relative_atomic_mass = 51.99616; break;
		case 25:relative_atomic_mass = 54.9380455; break;
		case 26:relative_atomic_mass = 55.8452; break;
		case 27:relative_atomic_mass = 58.9331955; break;
		case 28:relative_atomic_mass = 58.69342; break;
		case 29:relative_atomic_mass = 63.5463; break;
		case 30:relative_atomic_mass = 65.394; break;
		case 31:relative_atomic_mass = 69.7231; break;
		case 32:relative_atomic_mass = 72.641; break;
		case 33:relative_atomic_mass = 74.921602; break;
		case 34:relative_atomic_mass = 78.963; break;
		case 35:relative_atomic_mass = 79.9041; break;
		case 36:relative_atomic_mass = 83.7982; break;
		case 37:relative_atomic_mass = 85.46783; break;
		case 38:relative_atomic_mass = 87.621; break;
		case 39:relative_atomic_mass = 88.905852; break;
		case 40:relative_atomic_mass = 91.2242; break;
		case 41:relative_atomic_mass = 92.906382; break;
		case 42:relative_atomic_mass = 95.942; break;
		case 43:relative_atomic_mass = 97.9072; break;
		case 44:relative_atomic_mass = 101.072; break;
		case 45:relative_atomic_mass = 102.905502; break;
		case 46:relative_atomic_mass = 106.421; break;
		case 47:relative_atomic_mass = 107.86822; break;
		case 48:relative_atomic_mass = 112.4118; break;
		case 49:relative_atomic_mass = 114.8183; break;
		case 50:relative_atomic_mass = 118.7107; break;
		case 51:relative_atomic_mass = 121.7601; break;
		case 52:relative_atomic_mass = 127.603; break;
		case 53:relative_atomic_mass = 126.904473; break;
		case 54:relative_atomic_mass = 131.2936; break;
		case 55:relative_atomic_mass = 132.90545192; break;
		case 56:relative_atomic_mass = 137.3277; break;
		case 57:relative_atomic_mass = 137.3277; break;
		case 58:relative_atomic_mass = 140.1161; break;
		case 59:relative_atomic_mass = 140.907652; break;
		case 60:relative_atomic_mass = 144.2423; break;
		case 61:relative_atomic_mass = 145; break;
		case 62:relative_atomic_mass = 150.362; break;
		case 63:relative_atomic_mass = 151.9641; break;
		case 64:relative_atomic_mass = 157.253; break;
		case 65:relative_atomic_mass = 158.925352; break;
		case 66:relative_atomic_mass = 162.5001; break;
		case 67:relative_atomic_mass = 164.930322; break;
		case 68:relative_atomic_mass = 167.2593; break;
		case 69:relative_atomic_mass = 168.934212; break;
		case 70:relative_atomic_mass = 173.043; break;
		case 71:relative_atomic_mass = 174.9671; break;
		case 72:relative_atomic_mass = 178.492; break;
		case 73:relative_atomic_mass = 180.947882; break;
		case 74:relative_atomic_mass = 183.841; break;
		case 75:relative_atomic_mass = 186.2071; break;
		case 76:relative_atomic_mass = 190.233; break;
		case 77:relative_atomic_mass = 192.2173; break;
		case 78:relative_atomic_mass = 195.0849; break;
		case 79:relative_atomic_mass = 196.9665694; break;
		case 80:relative_atomic_mass = 200.592; break;
		case 81:relative_atomic_mass = 204.38332; break;
		case 82:relative_atomic_mass = 207.21; break;
		case 83:relative_atomic_mass = 208.980401; break;
		case 84:relative_atomic_mass = 208.9824; break;
		case 85:relative_atomic_mass = 209.9871; break;
		case 86:relative_atomic_mass = 222.0176; break;
		case 87:relative_atomic_mass = 223; break;
		case 88:relative_atomic_mass = 226; break;
		case 89:relative_atomic_mass = 227; break;
		case 90:relative_atomic_mass = 232.038062; break;
		case 91:relative_atomic_mass = 231.035882; break;
		case 92:relative_atomic_mass = 238.028913; break;
		case 93:relative_atomic_mass = 238.8486; break;
		case 94:relative_atomic_mass = 242.8798; break;
		case 95:relative_atomic_mass = 244.8594; break;
		case 96:relative_atomic_mass = 246.911; break;
		case 97:relative_atomic_mass = 248.9266; break;
		case 98:relative_atomic_mass = 252.9578; break;
		case 99:relative_atomic_mass = 253.9656; break;
		case 100:relative_atomic_mass = 259.0046; break;
		case 101:relative_atomic_mass = 260.0124; break;
		case 102:relative_atomic_mass = 261.0202; break;
		case 103:relative_atomic_mass = 264.0436; break;
		case 104:relative_atomic_mass = 269.0826; break;
		case 105:relative_atomic_mass = 270.0904; break;
		case 106:relative_atomic_mass = 273.1138; break;
		case 107:relative_atomic_mass = 274.1216; break;
		case 108:relative_atomic_mass = 272.106; break;
		case 109:relative_atomic_mass = 278.1528; break;
		case 110:relative_atomic_mass = 283.1918; break;
		case 111:relative_atomic_mass = 282.184; break;
		case 112:relative_atomic_mass = 287.223; break;
		default:relative_atomic_mass = 0; break;
		}
		return relative_atomic_mass;
	}

	double Get_electronegativity(int proton_num) {
		double res = 0;
		switch (proton_num) {
		case 1:res = 2.20; break;
		case 2:res = 3.89; break;
		case 3:res = 0.98; break;
		case 4:res = 1.57; break;
		case 5:res = 2.04; break;
		case 6:res = 2.55; break;
		case 7:res = 3.04; break;
		case 8:res = 3.44; break;
		case 9:res = 3.98; break;
		case 10:res = 3.67; break;
		case 11:res = 0.93; break;
		case 12:res = 1.31; break;
		case 13:res = 1.61; break;
		case 14:res = 1.90; break;
		case 15:res = 2.19; break;
		case 16:res = 2.58; break;
		case 17:res = 3.16; break;
		case 18:res = 3.3; break;
		case 19:res = 0.82; break;
		case 20:res = 1.00; break;
		case 21:res = 1.36; break;
		case 22:res = 1.54; break;
		case 23:res = 1.63; break;
		case 24:res = 1.66; break;
		case 25:res = 1.55; break;
		case 26:res = 1.83; break;
		case 27:res = 1.88; break;
		case 28:res = 1.91; break;
		case 29:res = 1.90; break;
		case 30:res = 1.65; break;
		case 31:res = 1.81; break;
		case 32:res = 2.01; break;
		case 33:res = 2.18; break;
		case 34:res = 2.55; break;
		case 35:res = 2.96; break;
		case 36:res = 3.00; break;
		case 37:res = 0.82; break;
		case 38:res = 0.95; break;
		case 39:res = 1.22; break;
		case 40:res = 1.33; break;
		case 41:res = 1.6; break;
		case 42:res = 2.16; break;
		case 43:res = 1.9; break;
		case 44:res = 2.2; break;
		case 45:res = 2.28; break;
		case 46:res = 2.20; break;
		case 47:res = 1.93; break;
		case 48:res = 1.69; break;
		case 49:res = 1.78; break;
		case 50:res = 1.96; break;
		case 51:res = 2.05; break;
		case 52:res = 2.1; break;
		case 53:res = 2.66; break;
		case 54:res = 2.67; break;
		case 55:res = 0.97; break;
		case 56:res = 0.89; break;
		case 57:res = 1.1; break;
		case 58:res = 1.12; break;
		case 59:res = 1.13; break;
		case 60:res = 1.14; break;
		case 61:res = 1.13; break;
		case 62:res = 1.17; break;
		case 63:res = 1.2; break;
		case 64:res = 1.2; break;
		case 65:res = 1.1; break;
		case 66:res = 1.22; break;
		case 67:res = 1.23; break;
		case 68:res = 1.24; break;
		case 69:res = 1.25; break;
		case 70:res = 1.1; break;
		case 71:res = 1.27; break;
		case 72:res = 1.3; break;
		case 73:res = 1.5; break;
		case 74:res = 2.36; break;
		case 75:res = 1.9; break;
		case 76:res = 2.2; break;
		case 77:res = 2.20; break;
		case 78:res = 2.28; break;
		case 79:res = 2.54; break;
		case 80:res = 2.00; break;
		case 81:res = 1.62; break;
		case 82:res = 2.33; break;
		case 83:res = 2.02; break;
		case 84:res = 2.0; break;
		case 85:res = 2.2; break;
		case 86:res = 2.2; break;
		case 87:res = 0.7; break;
		case 88:res = 0.9; break;
		case 89:res = 1.1; break;
		case 90:res = 1.3; break;
		case 91:res = 1.5; break;
		case 92:res = 1.38; break;
		case 93:res = 1.36; break;
		case 94:res = 1.28; break;
		case 95:res = 1.13; break;
		case 96:res = 1.28; break;
		case 97:res = 1.3; break;
		case 98:res = 1.3; break;
		case 99:res = 1.3; break;
		case 100:res = 1.3; break;
		case 101:res = 1.3; break;
		case 102:res = 1.3; break;
		case 103:res = 1.291; break;
		case 104:res = 1.3; break;
		case 105:res = 1.5; break;
		case 106:res = 2.36; break;
		case 107:res = 1.9; break;
		case 108:res = 2.2; break;
		case 109:res = 2.20; break;
		case 110:res = 2.28; break;
		case 111:res = 2.54; break;
		case 112:res = 2.00; break;
		default:res = -1; break;
		}
		return res;
	}

	string addHtml(string& tar) {
		replaceAllMark(tar, "·", "."); replaceAllMark(tar, "[", "(");
		replaceAllMark(tar, "{", "("); replaceAllMark(tar, "]", ")");
		replaceAllMark(tar, "}", ")");
		string res = "";
		int n = (int)tar.length();
		for (int i = 0; i < n; i++) {
			if (tar[i] == '.') {
				res += "·";
				if (i + 1 < n && tar[i + 1] >= '0' && tar[i + 1] <= '9') {
					i++;
					while (tar[i] >= '0' && tar[i] <= '9')
						res += tar[i++];
					i--;
				}
			}
			else if (tar[i] >= '0' && tar[i] <= '9') {
				res += "<sub>";
				while (tar[i] >= '0' && tar[i] <= '9')
					res += tar[i++];
				i--;

				res += "</sub>";
			}
			else
				res += tar[i];
		}
		return res;
	}

	void printMemory() {
#ifdef WINSYS
		HANDLE handle = GetCurrentProcess();
		PROCESS_MEMORY_COUNTERS pmc;
		GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
		cout << "当前占用内存 " << (int)pmc.WorkingSetSize / 8 / 1024 << " kB" << endl;
#else
		cout << "如果你在使用Windows操作系统，请在config.h中加上宏定义\"  #define WINSYS  \"。";
#endif

	}

	Fraction::Fraction() {
		a = 0; b = 1;
	}

	Fraction::Fraction(int x) {
		a = x; b = 1;
	}

	Fraction::~Fraction() {
		a = 0; b = 1;
	}

	void Fraction::reduce() {
		int x = gcd(a, b);
		a /= x, b /= x;
	};

	Fraction Fraction::operator = (int x) {
		a = x, b = 1;
		return *this;
	};

	Fraction Fraction::operator = (const Fraction x) {
		a = x.a; b = x.b;
		reduce();
		return *this;
	};

	Fraction Fraction::operator + (const Fraction x) {
		Fraction tmp;
		tmp.a = b * x.a + a * x.b; tmp.b = b * x.b;
		return tmp;
	};

	Fraction Fraction::operator - (const Fraction x) {
		Fraction tmp;
		tmp.a = a * x.b - b * x.a; tmp.b = b * x.b;
		return tmp;
	};

	Fraction Fraction::operator * (const Fraction x) {
		Fraction tmp;
		tmp.a = a * x.a; tmp.b = b * x.b;
		return tmp;
	};

	Fraction Fraction::operator / (const Fraction x) {
		Fraction tmp;
		tmp.a = a * x.b; tmp.b = b * x.a;
		return tmp;
	};

	bool Fraction::operator < (const Fraction x) {
		return a * x.b < b * x.a;
	};

	bool Fraction::operator <= (const Fraction x) {
		return a * x.b <= b * x.a;
	};

	bool Fraction::operator > (const Fraction x) {
		return a * x.b > b* x.a;
	};

	bool Fraction::operator >= (const Fraction x) {
		return a * x.b >= b * x.a;
	};

	bool Fraction::operator == (const Fraction x) {
		return a * x.b == b * x.a;
	};

	bool Fraction::operator != (const Fraction x) {
		return a * x.b != b * x.a;
	};

	void Fraction::a2f(char* tar) {
		char* ca = new char[MAXC];
		char* cb = new char[MAXC];
		int lentar = (int)strlen(tar);
		int i = 0; int ia = 0; int ib = 0; bool is_a = 1;
		while (i < lentar) {
			if (tar[i] == '/') {
				is_a = 0; i++;
			}
			else if (is_a)
				ca[ia++] = tar[i++];
			else
				cb[ib++] = tar[i++];
		}
		ca[ia] = '\0'; cb[ib] = '\0';
		a = atoi(ca); b = atoi(cb);
		delete[] ca;
		delete[] cb;
	}

	void Fraction::a2f(string& tar) {
		a2f((char*)tar.c_str());
	}

	string Fraction::p() {
		string fout;
		char* tmpa = new char[MAXC];
		char* tmpb = new char[MAXC];
		i2a((int)fabs(a), tmpa, 10); i2a((int)fabs(b), tmpb, 10);
		if (a == 0)
			fout = "0";
		else if (b == 1) {
			if (a > 0) {
				fout = "+"; fout += tmpa;
			}
			else {
				fout = "-"; fout += tmpa;
			}
		}
		else {
			if (a > 0) {
				fout = "+("; fout += tmpa; fout += "/"; fout += tmpb; fout += ")";
			}
			else {
				fout = "-("; fout += tmpa; fout += "/"; fout += tmpb; fout += ")";
			}
		}
		delete[]tmpa; delete[]tmpb;
		return fout;
	};

	int gcd(int x, int y) {
		return x % y == 0 ? y : gcd(y, x % y);
	}

	int lcm(int x, int y) {
		return x * y / gcd(x, y);
	}

	Fraction s2Frac(char* tar) {
		Fraction res;
		char* a = new char[MAXC];
		char* b = new char[MAXC];
		int len_tar = (int)strlen(tar);
		int i = 0; int ia = 0; int ib = 0; bool is_a = 1;
		while (i < len_tar) {
			if (tar[i] == '/') {
				is_a = 0; i++;
			}
			else if (is_a) {
				a[ia++] = tar[i++];
			}
			else {
				b[ib++] = tar[i++];
			}
		}
		a[ia] = '\0'; b[ib] = '\0';
		res.a = atoi(a); res.b = atoi(b);
		delete[]a; delete[]b;
		return res;
	}
	Fraction s2Frac(const string& tar) {
		return s2Frac((char*)tar.c_str());
	}

	Fraction Abs(Fraction x) {
		int p = x.a > 0 ? x.a : -x.a, q = x.b > 0 ? x.b : -x.b;
		Fraction tmp;
		tmp.a = p; tmp.b = q;
		return tmp;
	}

	void printMatrix(Fraction _fmatrix[][MAXM], int row, int col) {
		int i, j;
		cout << "     o-";
		for (j = 1; j <= col; j++)
			cout << "       ";
		cout << "           ";
		cout << "-o" << endl;
		for (i = 1; i <= row; i++) {
			cout << "     |";
			for (j = 1; j <= col; j++)
				printf("%8s", _fmatrix[i][j].p().c_str());
			cout << "      |" << endl;
		}
		cout << "     o_";
		for (j = 1; j <= col; j++)
			cout << "       ";
		cout << "           ";
		cout << "_o" << endl << endl;
	}

	bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]) {
		Fraction fres[MAXM];
		Fraction fmatrix[MAXM][MAXM];
		fres[col] = 1;							//  待定系数法 最后一个系数定为1
		for (int i = 1; i <= row; i++)					//  得到分数形式的系数矩阵					
			for (int j = 1; j <= col; j++)
				fmatrix[i][j] = imatrix[i][j];
		for (int i = 1; i <= row; i++)					//  最后一列取负作为结果列向量
			fmatrix[i][col].a = -fmatrix[i][col].a;
		Fraction zero = 0, maxm;
		int maxi, N = col - 1, K = row;
		for (int k = 1; k <= N; k++) {
			maxm.a = -1;  maxm.b = 1;
			maxi = 0;
			for (int i = k; i <= K; i++)
				if (maxm < Abs(fmatrix[i][k])) {
					maxm = Abs(fmatrix[i][k]); maxi = i;
				}
			if (maxm == zero)  return false;
			if (maxi != k)
				for (int j = 1; j <= N + 1; j++)
					std::swap(fmatrix[k][j], fmatrix[maxi][j]);
			Fraction tmp = fmatrix[k][k];
			for (int j = 1; j <= N + 1; j++)
				fmatrix[k][j] = fmatrix[k][j] / tmp;
			for (int i = k - 1 ? 1 : 2; i <= K; i++) {
				if (i == k)continue;
				tmp = fmatrix[i][k];
				for (int j = 1; j <= N + 1; j++)
					fmatrix[i][j] = fmatrix[i][j] - tmp * fmatrix[k][j];
			}
			//printMatrix(fmatrix, row, col);
		}

		for (int i = 1; i <= col - 1; i++)
			fres[i] = fmatrix[i][N + 1];
		int tmp2 = lcm(fres[1].b, fres[2].b);
		for (int i = 3; i <= col; i++)
			tmp2 = lcm(tmp2, fres[i].b);
		for (int i = 1; i <= col; i++)
			ires[i] = fres[i].a * tmp2 / fres[i].b;
		return true;
	}

	void Matrix_row_transformation(int m[][MAXM], int& row, int& col) {
		//  [1,row]  [1,col]
		int i = 0, j = 0, k = 0; Fraction ratio = 0, tmp = 0; bool ok = 0, first_not_zero = 0;
		for (i = 0; i <= row - 1; i++) {//  把第i行和第i+1~row行进行比较
			for (j = i + 1; j <= row; j++) {
				ok = 0; ratio = 0; first_not_zero = 1;
				for (k = 1; k <= col; k++) {//  扫描所有列
					if (m[i][k] == 0 && m[j][k] == 0)continue;
					else if (m[i][k] != 0 && m[j][k] != 0) {
						if (first_not_zero) {
							first_not_zero = 0;
							ratio = m[i][k] / m[j][k];
						}
						else {
							tmp = m[i][k] / m[j][k];
							if (ratio != tmp) {
								ok = 1; break;
							}
						}
					}
					else {//  同列的两个元素，一个为零，一个非零，这两行必然不成比例
						ok = 1; break;
					}
				}
				if (!ok) {
					for (k = 1; k <= col; k++)m[j][k] = m[row][k];
					row--;
					j--;
				}
			}
		}
	}

	Element::Element() {
		amount = 0; valence = 0;
		Reduced = 0; finalValence = 100;
	}

	Element::Element(string& _name) {
		name = _name; amount = 0; valence = 0;
		Reduced = 0; finalValence = 100;
	}

	Element::~Element() {
		clear();
	}

	void Element::clear() {
		name.clear(); amount = 0; valence = 0;
		Reduced = 0; finalValence = 100;
	}

	bool Element::Exist() {
		string tmp1 = "#H#He#Li#Be#B#C#N#O#F#Ne#Na#Mg#Al#Si#P#S#Cl#Ar#K#Ca#Sc#Ti#V#Cr#Mn#Fe#Co#Ni#Cu#Zn#Ga#Ge#As#Se#Br#Kr#Rb#Sr#Y#Zr#Nb#Mo#Tc#Ru#Rh#Pd#Ag#Cd#In#Sn#Sb#Te#I#Xe#Cs#Ba#La#Ce#Pr#Nd#Pm#Sm#Eu#Gd#Tb#Dy#Ho#Er#Tm#Yb#Lu#Hf#Ta#W#Re#Os#Ir#Pt#Au#Hg#Tl#Pb#Bi#Po#At#Rn#Fr#Ra#Ac#Th#Pa#U#Np#Pu#Am#Cm#Bk#Cf#Es#Fm#Md#No#Lr#Rf#Db#Sg#Bh#Hs#Mt#Ds#Rg#Cn#Uut#Fl#Uup#Lv#Uuo#";
		string tmp2 = "#" + name + "#";
		if (tmp1.find(tmp2) == string::npos)
			return 0;
		else
			return 1;
	}


	MatNode::MatNode() {
		num = 0; isHead = 0; isNum = 0;
	}

	MatNode::~MatNode() {
		clear();
	}

	void MatNode::clear() {
		block.clear(); num = 0; isHead = 0; isNum = 0;
	}

	void MatNode::print() {
		if (isNum) {
			cout << "[\"" << "isNum\", \"num=" << num << "\"]" << "---";
		}
		else {
			cout << "[\"" << "isEle\", \"ele=";
			for (int i = 0; i < (int)block.size(); i++)
				cout << block[i].name << ",";
			cout << "\"]" << "---";
		}
	}

	bool namelessthan(const Element& a, const Element& b) {
		return a.name < b.name;
	}

	void MatNode::compress() {	//  压缩元素阵列 合并相同的元素 累加元素数量
		sort(block.begin(), block.end(), namelessthan);
		int j = 0;
		for (int i = 0; i < (int)block.size() - 1; i++) {
			j = i + 1;
			while (j < (int)block.size() && block[i].name == block[j].name) {
				block[i].amount += block[j].amount;
				j++;
			}
			block.erase(block.begin() + i + 1, block.begin() + j);
		}
	}

	Matter::Matter() {
		amount = 0; state = 'l'; q = 0;
	}

	Matter::Matter(string& _name) {
		name = _name; amount = 0; state = 'l'; q = 0;
	}

	Matter::Matter(char* _name) {
		name = _name; amount = 0; state = 'l'; q = 0;
	}

	Matter::~Matter() {
		clear();
	}

	void Matter::clear() {
		name.clear(); stateInfo.clear(); eleList.clear();
		amount = 0; state = 'l'; q = 0;
	}

	void Matter::inputName() {
		getline(std::cin, name);
	}

	bool Matter::Exist() {
		string tmp1 = "#CCl4#CH3Cl#C2H5OC2H5#KO2#CuCl2#CHCl3#CH3COONa#CH3COOCH3#CH3COOC2H5#CH3CH2Br#CH2Cl2#C2H5ONa#(C2H5O)2Mg#C2H5ONO2#CH3CH2OSO3H#(C2H5O)3Al#CH3CH2OH#C11H22O11#Ba(NO3)2#Al2S3#NaAlO2#NH3·H2O#AgOH#Ca(ClO)2#Ca(H2PO4)2#CH4#C2H6#C2H4#C2H2#Fe(NO3)2#CaC2#CH3CHO#C2H5OH#Fe(OH)2#Ca(HCO3)2#CH3OH#Ag(NH3)2OH#HCHO#HClO#K2MnO4#NaH#Mg(HCO3)2#Mg3N2#MgSiO3#NO2#NO#N2O4#Na2SO4#Cu(OH)2#H4SiO4#H2SiO3#NaBr#NaHS#NaHSO3#NaHSO4#Cu3P#HPO3#PCl5#PCl3#C6H5SO3H#C6H12#C6H6#C6H5Br#C6H5Cl#CS2#K2S#Ca(HSO3)2#CaSO3#CaSiO3#ZrOCl2#ZrO2#Zr(SO4)2#Zr(NO3)4#Zr#ZnSO4#ZnS#ZnO#ZnCl2#ZnCO3#ZnC2O4#Zn2P2O7#Zn(NO3)2#Zn#WO3#W#VO2#V2O5#V#UO3#UF4#UCl4#U3O8#U#TiOSO4#TiO2#TiCl4#TiCl3#Ti#ThCl4#Th(SO4)2#Th(NO3)4#Th(C2O4)2#Th#SrSO4#SrO#SrCl2#SrCO3#SrC2O4#Sr3(PO4)2#Sr(NO3)2#Sr#SnS2#SnS#SnO2#SnCl2#Sn#SiO2#SiF4#SiCl4#Si#SbCl5#SbCl3#Sb2O5#Sb2O3#Sb#SO3#SO2#S#PbbI2#PbSO4#PbS#PbO2#PbO#PbCrO4#PbCl2#PbCO3#PbC2O4#Pb3O4#Pb3(PO4)2#Pb2O3#Pb(NO3)2#Pb(IO3)2#Pb#P2O5#P#O3#O2#O#NiSO4#NiS#NiO#NiCl2#Ni(NO3)2#Ni#NaOH#NaNO3#NaNO2#NaI#NaHCO3#NaH2PO4#NaH#NaClO#NaCl#NaCO3#NaCN#NaBrO3#NaBiO3#NaAsO4#Na3PO4#Na3AsO4#Na3AsO3#Na2SO4#Na2SiO3#Na2SO3#Na2S2O3#Na2S#Na2O2#Na2O#Na2HPO4#Na2HAsO3#Na2CrO4#Na2CO3#Na2C2O4#Na2B4O7#Na#NH4VO3#NH4NO3#NH4HCO3#NH4H2PO4#NH4Cl#NH3#N2#N#MnSO4#MnS#MnO2#MnO#MnCl2#MnCO3#Mn3O4#Mn2P2O7#Mn2O3#Mn(NO3)2#Mn#MgSO4#MgO#MgNH4PO4#MgNH4AsO4#MgH2#MgCl2#MgCO3#MgC2O4#Mg2P2O7#Mg(OH)2#Mg(NO3)2#Mg#LiOH#LiH#LiCl#Li2O#Li2CO3#Li#KOH#KNaC4O6H4#KNO3#KNO2#KMnO4#KIO3#KI#KHSO4#KHC8O4H8#KHC4O6H4#KHC2O4#KH#KFe(SO4)2#KF#KClO4#KClO3#KCl#KBrO3#KBr#KAl(SO4)2#K4Fe(CN)6#K3PO4#K3Fe(CN)6#K3AsO4#K2SO4#K2S2O7#K2PtCl6#K2O#K2CrO4#K2Cr2O7#K2CO3#K#I2#I#HgSO4#HgS#HgO#HgI2#HgCl2#Hg2SO4#Hg2I2#Hg2Cl2#Hg2Br2#Hg2(NO3)2#Hg(NO3)2#Hg(CN)2#Hg#HNO3#HNO2#HIO3#HI#HF#HClO4#HCl#HCOONa#HCOOH#HCN#HBr#H3PO4#H3PO3#H3BO3#H3AsO4#H3AsO3#H2SO4#H2SO3#H2S#H2O2#H2O#H2CO3#H2C2O4#H2#H#FeSO4#FeS2#FeS#FeO#FeCl3#FeCl2#FeCO3#Fe3O4#Fe2O3#Fe2(SO4)3#Fe(OH)3#Fe(NO3)3#Fe#F2#F#CuSO4#CuSCN#CuS#CuO#CuCl2CuI#CuCl#Cu2S#Cu2O#Cu2(OH)2CO3#Cu(NO3)2#Cu#CsOH#Cs#CrCl3#Cr2O3#Cr2(SO4)3#Cr#CoSO4#CoS#CoCl2#Co3O4#Co2O3#Co(NO3)2#Co#Cl2#Cl#CeO2#Ce(SO4)2#Ce#CdS#CdCl2#CdCO3#Cd#CaSO4#CaO#CaCl2#CaCO3#CaC2O4#Ca3(PO4)2#Ca(OH)2#Ca#CS(NH2)2#CO2#CO(NH2)2#CO#CH3COONH4#CH3COOH#C6H5SO3H#C6H5ONa#C6H5OH#C6H5NO2#C6H5Cl#C6H5COOH#C6H5Br#C6H12#C#Br2#Br#BiONO3#BiOHCO3#BiOCl#BiCl3#Bi2S3#Bi2O3#Bi(NO3)3#Bi#BeO#Be#BaSO4#BaO#BaCrO4#BaCl2#BaCO3#BaBr2#Ba3(AsO4)2#Ba(OH)2#Ba#B2O3#B#As2S3#As2O5#As2O3#As#AlCl3#AlBr3#Al2O3#Al2(SO4)3#Al(OH)3#Al(NO3)3#Al#AgSCN#AgNO3#AgI#AgCl#AgCN#AgBrO3#AgBr#Ag3AsO4#Ag2SO4#Ag2CrO4#Ag#(NH4)3PO4#(NH4)2SO4#(NH4)2S#(NH4)2PtCl6#(NH4)2MoO4#(NH4)2HPO4#(NH4)2Fe(SO4)2#(NH4)2CO3#(NH4)2C2O4#(CH3COO)2UO2#(CH3COO)2Pb#(CH3CO)2O#(CH2COO)2Zn#";
		string tmp2 = "#" + name + "#";
		if (tmp1.find(tmp2) == string::npos)
			return 0;
		else
			return 1;
	}

	void Matter::getStateInfo() {
		string tmp1 = "#ZnSO4#r#ZnCl2#r#Zn(NO3)2#r#Zn#m#X#x#WO3#s#W#m#V2O5#s#V#m#SiO2#s#SiF4#g#Si#s#SO3#l#SO2#g#S#s#PhSO3H#o#PhONa#r#PhOH#s#PhNO2#o#PhCl#o#PhBr#o#Ph#o#PbS#s#PbAc2#r#PCl5#s#PCl3#s#P2O5#s#P#s#O2#g#NaSO4#r#NaOH#r#NaNO3#r#NaNO2#r#NaHSO4#r#NaHSO3#r#NaHS#r#NaHCO3#r#NaClO#r#NaCl#r#NaBr#r#NaAlO2#r#Na2SiO3#r#Na2SO4#r#Na2SO3#r#Na2S#r#Na2O2#s#Na2O#s#Na2CO3#r#Na#m#NO2#g#NO#g#NH4NO3#r#NH4HCO3#r#NH4Cl#r#NH3·H2O#l#NH3#g#N2O4#g#N2#g#MnSO4#r#MnO2#s#MnO#s#MnCl2#r#Mn#m#MgSiO3#s#MgSO4#r#MgO#s#MgCl2#r#MgCO3#s#Mg3N2#s#Mg(OH)2#s#Mg(NO3)2#r#Mg(HCO3)2#r#Mg#m#MX#r#MOH#r#MH#s#M#m#Li2O#s#Li#m#KOH#r#KO2#s#KNO3#r#KMnO4#r#KI#r#KClO3#r#KCl#r#KBr#r#K2SO4#r#K2S#r#K2MnO4#r#KAl(SO4)2#r#K2Cr2O7#r#K#m#I2#s#HgO#s#Hg(NO3)2#r#Hg#s#HPO3#s#HNO3#r#HI#r#HF#r#HClO#l#HCl#r#HCIO#l#HCHO#o#HBr#r#HAc#l#H4SiO4#s#H3PO4#l#H2SiO3#s#H2SO4#r#H2SO3#l#H2S#g#H2O2#l#H2O#l#H2CO3#l#H2#g#FeSO4#r#FeS2#s#FeS#s#FeO#s#FeCl3#l#FeCl2#r#Fe3O4#s#Fe2O3#s#Fe2(SO4)3#r#FeSO4#r#Fe(OH)3#s#Fe(OH)2#s#Fe(NO3)2#r#Fe#m#F2#g#CuSO4#r#CuS#s#CuO#s#CuCl2#r#Cu3P#s#Cu2S#s#Cu2O#s#Cu(OH)2#s#Cu(NO3)2#r#Cu#s#Cr2O3#s#Cr2(SO4)3#r#Cr#m#Cl2#g#CaSiO3#s#CaSO4#s#CaSO3#s#CaO#s#CaCl2#r#CaCO3#s#CaC2#s#Ca3(PO4)2#s#Ca(OH)2#r#Ca(HSO3)2#r#Ca(HCO3)2#r#Ca(H2PO4)2#r#Ca(ClO)2#r#CS2#l#CO2#g#CO#g#CHCl3#l#CH4#g#CH3OH#l#CH3Cl#g#CH3COONa#r#CH3COONH4#r#CH3COOH#l#CH3COOCH3#l#CH3COOC2H5#l#CH3CHO#l#CH3CH2OSO3H#o#CH3CH2OH#l#CH3CH2Br#o#CH2Cl2#o#CCl4#l#C6H6#l#C2H6#g#C2H5ONa#r#C2H5ONO2#o#C2H5OH#l#C2H5OC2H5#o#C2H4#g#C2H2#g#C11H22O11#s#C#s#Br2#l#BaSO4#s#BaCl2#r#BaCO3#s#Ba(NO3)2#r#AlCl3#r#Al2S3#s#Al2O3#s#Al2(SO4)3#r#Al(OH)3#s#Al(NO3)3#r#Al#m#AgOH#s#AgNO3#r#AgI#s#AgCl#s#AgBr#s#Ag(NH3)2OH#r#Ag#s#(NH4)2SO4#r#(CH3COO)2Pb#r#(C2H5O)3Al#r#(C2H5O)2Mg#r####";
		string tmp2 = "#" + name + "#";
		size_t index = tmp1.find(tmp2);
		if (index == string::npos)
			state = 'g';//找不到，初始化为气体
		else {
			index++;
			while (tmp1[index] != '#')
				index++;
			state = tmp1[index + 1];
		}
	}

	bool Matter::intoElement() {
		//  格式预处理
		string tar = name;
		if (!preProcess(tar)) {
#ifdef TSETCEI
			cout << "数据预处理失败！处理前为 \""
				<< name << "\" ；处理后为 \"" << tar << "\" 。" << endl;
#endif
			return 0;
		}
		//  词法分析
		int i = 0, j = 0, k = 0;
		vector<MatNode> matlist;
		if (!lexicalAnalysis(matlist, tar)) {
#ifdef TESTCEI
			cout << "词法分析失败！处理前为 \"" << name << "\" ，已处理的信息：" << endl;
			cout << "(int)matlist.size=" << (int)matlist.size() << ", matlist=";
			for (i = 0; i < (int)matlist.size(); i++)
				matlist[i].print();
#endif
			return 0;
		}
		//  进一步词法分析，循环处理最小单元
		furtherLexicalAnalysis(matlist);
		//  存入元素列表
		eleList.clear();
		for (i = 0; i < (int)matlist[0].block.size(); i++)
			eleList.emplace_back(matlist[0].block[i]);
		matlist.clear();
#ifdef TESTCEI
		cout << "物质解析完成，M(" << name << ") = " << getRelativeMolecureMass() << " g/mol" << endl;
#endif
		return 1;
	}

	void Matter::printElement() {
		size_t i = 0; size_t col = 1; size_t leneleList = eleList.size();
		for (i = 0; i < leneleList; i++)
			col = (col > eleList[i].name.length() ? col : eleList[i].name.length());
		for (i = 0; i < leneleList; i++)
			cout << name << " 的元素 " << std::setw(col) << eleList[i].name << " 有 " << std::setw(4) << eleList[i].amount << "份" << endl;
	}

	bool Matter::getValence() {
		string tmp1 = "#CCl4#4/1#-1/1#CH3Cl#-2/1#-1/1#1/1#C2H5OC2H5#-2/1#1/1#-2/1#KO2#1/1#-1/2#CuCl2#-1/1#2/1#CHCl3#2/1#-1/1#1/1#CH3Cl#-2/1#-1/1#1/1#CH3COONa#0/1#1/1#1/1#-2/1#CH3COOCH3#-2/3#1/1#-2/1#CH3COOC2H5#-1/1#1/1#-2/1#CH3CH2Br#-1/1#-2/1#1/1#CH2Cl2#0/1#-1/1#1/1#C2H5ONa#-2/1#1/1#1/1#-2/1#(C2H5O)2Mg#-2/1#1/1#2/1#-2/1#C2H5ONO2#-2/1#1/1#5/1#-2/1#CH3CH2OSO3H#-2/1#1/1#-2/1#6/1#(C2H5O)3Al#3/1#-2/1#1/1#-2/1#CH3CH2OH#-2/1#1/1#-2/1#C11H22O11#0/1#1/1#-2/1#Ba(NO3)2#2/1#5/1#-2/1#Al2S3#3/1#-2/1#NaAlO2#3/1#1/1#-2/1#NH3·H2O#1/1#-3/1#-2/1#AgOH#1/1#1/1#-2/1#Ca(ClO)2#2/1#1/1#-2/1#Ca(H2PO4)2#2/1#1/1#-2/1#5/1#CH4#-4/1#1/1#C2H6#-3/1#1/1#C2H4#-2/1#1/1#C2H2#-1/1#1/1#Fe(NO3)2#2/1#5/1#-2/1#CaC2#-1/1#2/1#CH3CHO#-1/1#1/1#-2/1#C2H5OH#-2/1#1/1#-2/1#Fe(OH)2#2/1#1/1#-2/1#Ca(HCO3)2#4/1#2/1#1/1#-2/1#CH3OH#-2/1#1/1#-2/1#Ag(NH3)2OH#1/1#1/1#-3/1#-2/1#HCHO#0/1#1/1#-2/1#HClO#1/1#1/1#-2/1#MgH2#-1/1#2/1#K2MnO4#1/1#6/1#-2/1#KH#-1/1#1/1#LiH#-1/1#1/1#NaH#-1/1#1/1#Cs#0/1#CsOH#1/1#1/1#-2/1#KF#-1/1#1/1#Mg(HCO3)2#4/1#1/1#2/1#-2/1#Mg3N2#2/1#-3/1#MgSiO3#2/1#-2/1#4/1#N2O4#4/1#-2/1#NO#2/1#-2/1#NO2#4/1#-2/1#Na2SO4#1/1#-2/1#6/1#Cu(OH)2#2/1#1/1#-2/1#H4SiO4#1/1#-2/1#4/1#H2SiO3#1/1#-2/1#4/1#NaBr#-1/1#1/1#NaHS#1/1#1/1#-2/1#NaHSO3#1/1#1/1#-2/1#4/1#NaHSO4#1/1#1/1#-2/1#6/1#Cu3P#1/1#-3/1#HPO3#1/1#-2/1#5/1#PCl5#-1/1#5/1#PCl3#-1/1#3/1#C6H5SO3H#-2/3#1/1#-2/1#4/1#C6H12#-2/1#1/1#C6H5NO2#-2/3#1/1#3/1#-2/1#C6H6#-1/1#1/1#C6H5Br#-2/3#-1/1#1/1#C6H5Cl#-2/3#-1/1#1/1#C6H5OH#-2/3#1/1#-2/1#C6H5ONa#-2/3#1/1#1/1#-2/1#CS2#4/1#-2/1#K2S#1/1#-2/1#Ca(HSO3)2#2/1#1/1#-2/1#4/1#CaSO3#2/1#-2/1#4/1#CaSiO3#2/1#-2/1#4/1#Na2SiO3#1/1#-2/1#4/1#Ag#0/1#AgBr#1/1#-1/1#AgBrO3#1/1#5/1#-2/1#AgCl#1/1#-1/1#AgI#1/1#-1/1#AgNO3#1/1#5/1#-2/1#Ag2CrO4#1/1#6/1#-2/1#Ag2SO4#1/1#-2/1#6/1#Ag3AsO4#1/1#5/1#-2/1#Al#0/1#AlBr3#3/1#-1/1#AlCl3#3/1#-1/1#Al(NO3)3#3/1#5/1#-2/1#Al2O3#3/1#-2/1#Al(OH)3#3/1#1/1#-2/1#Al2(SO4)3#3/1#-2/1#6/1#As#0/1#As2O3#3/1#-2/1#As2O5#5/1#-2/1#H3AsO4#5/1#1/1#-2/1#B#0/1#B2O3#3/1#-2/1#Ba#0/1#BaBr2#2/1#-1/1#BaCO3#2/1#4/1#-2/1#BaCl2#2/1#-1/1#BaCrO4#2/1#6/1#-2/1#BaO#2/1#-2/1#Ba(OH)2#2/1#1/1#-2/1#BaSO4#2/1#-2/1#6/1#Ba3(AsO4)2#5/1#2/1#-2/1#Be#0/1#BeO#2/1#-2/1#Bi#0/1#Bi2O3#3/1#-2/1#Br#0/1#NaBrO3#5/1#1/1#-2/1#Br2#0/1#C#0/1#CH3COOH#0/1#1/1#-2/1#(CH3CO)2O#0/1#1/1#-2/1#NaAsO4#7/1#1/1#-2/1#CO#2/1#-2/1#CO2#4/1#-2/1#Na2C2O4#3/1#1/1#-2/1#Ca#0/1#CaCl2#2/1#-1/1#CaCO3#4/1#2/1#-2/1#CaC2O4#3/1#2/1#-2/1#CaO#2/1#-2/1#Ca(OH)2#2/1#1/1#-2/1#CaSO4#2/1#-2/1#6/1#Ca3(PO4)2#2/1#-2/1#5/1#Cd#0/1#Ce#0/1#CeO2#4/1#-2/1#Cl2#0/1#Cl#0/1#Co#0/1#Cr#0/1#Na2CrO4#6/1#1/1#-2/1#Cr2O3#3/1#-2/1#Cu#0/1#CuO#2/1#-2/1#Cu2O#1/1#-2/1#F#0/1#F2#0/1#Fe#0/1#FeO#2/1#-2/1#Fe(OH)3#3/1#1/1#-2/1#Fe2O3#3/1#-2/1#Fe3O4#8/3#-2/1#H#0/1#HBr#-1/1#1/1#HCOOH#2/1#1/1#-2/1#CH3COOH#0/1#1/1#-2/1#C6H5COOH#-2/7#1/1#-2/1#HCl#-1/1#1/1#HClO4#7/1#1/1#-2/1#HF#-1/1#1/1#HI#1/1#-1/1#HIO3#1/1#5/1#-2/1#HNO2#1/1#3/1#-2/1#H2#0/1#H2CO3#4/1#1/1#-2/1#H2C2O4#3/1#1/1#-2/1#H2O#1/1#-2/1#H2S#1/1#-2/1#H2SO3#1/1#-2/1#4/1#H2SO4#1/1#-2/1#6/1#H3AsO3#3/1#1/1#-2/1#H3AsO4#5/1#1/1#-2/1#H3BO3#3/1#1/1#-2/1#H3PO3#1/1#-2/1#3/1#H3PO4#1/1#-2/1#5/1#Hg#0/1#HgO#2/1#-2/1#I#0/1#I2#0/1#K#0/1#KAl(SO4)2#3/1#1/1#-2/1#6/1#KBr#-1/1#1/1#KBrO3#5/1#1/1#-2/1#KCl#-1/1#1/1#KClO3#5/1#1/1#-2/1#KClO4#7/1#1/1#-2/1#KHC2O4#3/1#1/1#1/1#-2/1#KHC4O6H4#3/2#1/1#1/1#-2/1#KHC8O4H8#1/-4#1/1#1/1#-2/1#KHSO4#1/1#1/1#-2/1#6/1#KI#-1/1#1/1#KIO3#5/1#1/1#-2/1#KMnO4#1/1#7/1#-2/1#KNO2#1/1#3/1#-2/1#KNO3#1/1#5/1#-2/1#KNaC4O6H4#3/2#1/1#1/1#1/1#-2/1#KOH#1/1#1/1#-2/1#K2CO3#4/1#1/1#-2/1#K2CrO4#6/1#1/1#-2/1#K2Cr2O7#6/1#1/1#-2/1#K2SO4#1/1#-2/1#6/1#K2S2O7#1/1#-2/1#6/1#K3AsO4#5/1#1/1#-2/1#K3PO4#1/1#-2/1#5/1#Li#0/1#LiCl#-1/1#1/1#LiOH#1/1#1/1#-2/1#Li2CO3#4/1#1/1#-2/1#Li2O#1/1#-2/1#Mg#0/1#MgCO3#4/1#2/1#-2/1#MgC2O4#3/1#2/1#-2/1#MgCl2#-1/1#2/1#Mg(NO3)2#2/1#5/1#-2/1#MgO#2/1#-2/1#Mg(OH)2#1/1#2/1#-2/1#MgSO4#2/1#-2/1#6/1#Mg2P2O7#2/1#-2/1#5/1#Mn#0/1#MnO#2/1#-2/1#MnO2#4/1#-2/1#Mn2O3#3/1#-2/1#Mn3O4#8/3#-2/1#N#0/1#N2#0/1#NH3#1/1#-3/1#HNO3#1/1#5/1#-2/1#NaNO3#5/1#1/1#-2/1#Na#0/1#NaBiO3#5/1#1/1#-2/1#Na#0/1#NaBrO3#5/1#1/1#-2/1#HCOONa#2/1#1/1#1/1#-2/1#NaCl#-1/1#1/1#NaClO#1/1#1/1#-2/1#NaHCO3#4/1#1/1#1/1#-2/1#NaH2PO4#1/1#1/1#-2/1#5/1#NaI#-1/1#1/1#NaNO2#3/1#1/1#-2/1#NaNO3#5/1#1/1#-2/1#NaOH#1/1#1/1#-2/1#Na2B4O7#3/1#1/1#-2/1#Na2CO3#4/1#1/1#-2/1#Na2C2O4#3/1#1/1#-2/1#Na2HAsO3#3/1#1/1#1/1#-2/1#Na2HPO4#1/1#1/1#-2/1#5/1#Na2O#1/1#-2/1#Na2S#1/1#-2/1#Na2SO3#1/1#-2/1#4/1#Na2SO4#1/1#-2/1#6/1#Na2S2O3#1/1#-2/1#2/1#Na3AsO3#3/1#1/1#-2/1#Na3AsO4#5/1#1/1#-2/1#Na3PO4#1/1#-2/1#5/1#Ni#0/1#NiO#2/1#-2/1#O#0/1#H2O#1/1#-2/1#O2#0/1#O3#0/1#P#0/1#H3PO4#1/1#-2/1#5/1#P2O5#-2/1#5/1#Pb#0/1#PbO#-2/1#2/1#PbO2#-2/1#4/1#Pb2O3#-2/1#3/1#Pb3O4#-2/1#8/3#S#0/1#SO2#-2/1#4/1#SO3#-2/1#6/1#Na2SO4#1/1#-2/1#6/1#Sb#0/1#Sb2O3#-2/1#3/1#Sb2O5#-2/1#5/1#Si#0/1#SiF4#-1/1#4/1#SiO2#-2/1#4/1#Sn#0/1#SnO2#-2/1#4/1#Sr#0/1#SrO#-2/1#2/1#Th#0/1#Ti#0/1#TiO2#-2/1#4/1#U#0/1#UF4#-1/1#4/1#UO3#-2/1#6/1#U3O8#-2/1#16/3#V#0/1#VO2#-2/1#4/1#V2O5#-2/1#5/1#W#0/1#WO3#-2/1#6/1#Zn#0/1#ZnCO3#4/1#-2/1#2/1#ZnC2O4#3/1#-2/1#2/1#(CH2COO)2Zn#1/2#1/1#-2/1#2/1#ZnCl2#-1/1#2/1#Zn(NO3)2#5/1#-2/1#2/1#ZnO#-2/1#2/1#ZnS#-2/1#2/1#ZnSO4#-2/1#6/1#2/1#Zn2P2O7#-2/1#5/1#2/1#Zr#0/1#ZrO2#-2/1#4/1#KAl(SO4)2#3/1#1/1#-2/1#6/1#BiCl3#3/1#-1/1#Bi(NO3)3#3/1#5/1#-2/1#BiOCl#3/1#-1/1#-2/1#BiONO3#3/1#5/1#-2/1#NaCO3#4/1#1/1#-2/1#CS(NH2)2#4/1#1/1#-3/1#-2/1#CdS#2/1#-2/1#CoCl2#-1/1#2/1#CdCl2#2/1#-1/1#Co(NO3)2#2/1#5/1#-2/1#CoSO4#2/1#-2/1#6/1#Co2O3#3/1#-2/1#Co3O4#8/3#-2/1#CrCl3#-1/1#3/1#CuCl#-1/1#1/1#CuCl2#-1/1#2/1#CuI#1/1#-1/1#Cu(NO3)2#2/1#5/1#-2/1#CuSO4#2/1#-2/1#6/1#Cu2S#1/1#-2/1#FeCO3#4/1#2/1#-2/1#FeCl2#-1/1#2/1#FeCl3#-1/1#3/1#Fe(NO3)3#3/1#5/1#-2/1#FeSO4#2/1#-2/1#6/1#Fe2(SO4)3#3/1#-2/1#6/1#HgCl2#-1/1#2/1#HgI2#2/1#-1/1#Hg(NO3)2#2/1#5/1#-2/1#HgSO4#2/1#-2/1#6/1#Hg2Br2#-1/1#1/1#Hg2Cl2#-1/1#1/1#Hg2I2#1/1#-1/1#Hg2(NO3)2#1/1#5/1#-2/1#Hg2SO4#1/1#-2/1#6/1#KFe(SO4)2#3/1#1/1#-2/1#6/1#K3Fe(CN)6#2/1#3/1#1/1#-3/1#K4Fe(CN)6#2/1#2/1#1/1#-3/1#MnCO3#4/1#2/1#-2/1#MnCl2#-1/1#2/1#Mn(NO3)2#2/1#5/1#-2/1#MnSO4#2/1#-2/1#6/1#NiCl2#-1/1#2/1#Ni(NO3)2#5/1#2/1#-2/1#NiSO4#2/1#-2/1#6/1#PbCO3#4/1#-2/1#2/1#PbC2O4#3/1#-2/1#2/1#PbCl2#-1/1#2/1#PbI2#-1/1#2/1#Pb(NO3)2#5/1#-2/1#2/1#PbSO4#-2/1#2/1#6/1#Pb3(PO4)2#-2/1#5/1#2/1#SbCl3#-1/1#3/1#SbCl5#-1/1#5/1#SiCl4#-1/1#4/1#SnCl2#-1/1#2/1#SrCO3#4/1#-2/1#2/1#SrCl2#-1/1#2/1#Sr(NO3)2#5/1#-2/1#2/1#SrSO4#-2/1#6/1#2/1#Sr3(PO4)2#-2/1#5/1#2/1#ThCl4#-1/1#4/1#Th(NO3)4#5/1#-2/1#4/1#Th(SO4)2#-2/1#6/1#4/1#TiCl3#-1/1#3/1#TiCl4#-1/1#4/1#TiOSO4#-2/1#6/1#4/1#UCl4#-1/1#4/1#Zr(NO3)4#5/1#-2/1#4/1#ZrOCl2#-1/1#-2/1#4/1#Zr(SO4)2#-2/1#6/1#4/1#BiOHCO3#3/1#4/1#1/1#-2/1#CdCO3#4/1#2/1#-2/1#Ce(SO4)2#4/1#-2/1#6/1#Cr2(SO4)3#3/1#-2/1#6/1#Cu2(OH)2CO3#4/1#2/1#1/1#-2/1#Mn2P2O7#2/1#-2/1#5/1#PbCrO4#6/1#-2/1#2/1#AgCN#1/1#2/1#-3/1#AgSCN#1/1#4/1#-3/1#-2/1#Bi2S3#3/1#-2/1#HCN#2/1#1/1#-3/1#NaCN#2/1#-3/1#1/1#CO(NH2)2#4/1#1/1#-3/1#-2/1#CoS#2/1#-2/1#CuS#2/1#-2/1#(NH4)2Fe(SO4)2#2/1#1/1#-3/1#-2/1#6/1#FeS#2/1#-2/1#FeS2#4/1#-2/1#HCN#2/1#1/1#-3/1#HgS#2/1#-2/1#K2PtCl6#-1/1#1/1#4/1#MgNH4AsO4#5/1#1/1#2/1#-3/1#-2/1#MgNH4PO4#1/1#2/1#-3/1#-2/1#5/1#MnS#2/1#-2/1#CH3COONH4#0/1#1/1#-3/1#-2/1#NH4Cl#-1/1#1/1#-3/1#NH4HCO3#4/1#1/1#-3/1#-2/1#NH4H2PO4#1/1#-3/1#-2/1#5/1#NH4NO3#1/1#1/1#-2/1#NH4VO3#1/1#-3/1#-2/1#5/1#(NH4)2CO3#4/1#1/1#-3/1#-2/1#(NH4)2C2O4#3/1#1/1#-3/1#-2/1#(NH4)2HPO4#1/1#-3/1#-2/1#5/1#(NH4)2MoO4#1/1#6/1#-3/1#-2/1#(NH4)2PtCl6#-1/1#1/1#-3/1#4/1#(NH4)2S#1/1#-3/1#-2/1#(NH4)2SO4#1/1#-3/1#-2/1#-2/1#(NH4)3PO4#1/1#-3/1#-2/1#5/1#NiS#2/1#-2/1#PbS#2/1#-2/1#SnS#-2/1#2/1#SnS2#-2/1#4/1#SrC2O4#3/1#-2/1#2/1#(CH3COO)2Pb#0/1#1/1#-2/1#2/1#As2S3#3/1#-2/1#Pb(IO3)2#5/1#2/1#-2/1#CuSCN#4/1#1/1#-3/1#-2/1#Hg(CN)2#2/1#2/1#-3/1#Hg2(CN)2#2/1#1/1#-3/1#Th(C2O4)2#3/1#-2/1#4/1#(CH3COO)2UO2#0/1#1/1#-2/1#6/1#H2O2#1/1#-1/1#K2O#1/1#-2/1#KO2#1/1#-1/2#BaO2#2/1#-1/1#K2O2#1/1#-1/1#Na2O2#1/1#-1/1#";
		string tmp2 = "#" + name + "#";
		size_t index = tmp1.find(tmp2);
		if (index == string::npos)
			return 0;
		index += tmp2.length();
		int ival = 0, i = 0;
		char* cval = new char[MAXC];
		for (i = 0; i < (int)eleList.size(); i++) {
			ival = 0;
			while (tmp1[index] != '#')
				cval[ival++] = tmp1[index++];
			index++; cval[ival] = '\0';
			eleList[i].valence.a2f(cval);
		}
		delete[]cval;
		return 1;
	}

	void Matter::printValence() {
		size_t i = 0;
		size_t col = 1;
		size_t leneleList = eleList.size();
		for (i = 0; i < leneleList; i++)
			col = (col > eleList[i].name.length() ? col : eleList[i].name.length());
		for (i = 0; i < leneleList; i++)
			cout << name << " 的元素 " << std::setw(col) << eleList[i].name << " 平均价态 " << std::setw(8) << eleList[i].valence.p() << endl;
	}

	bool Matter::lexicalAnalysis(vector<MatNode>& matlist, string& tar) {
		Element eletmp;
		MatNode tmpnode;
		int tarlen = (int)tar.length();
		int i = 0, iend = 0, j = 0, amount = 0;
		for (i = 0; i < tarlen; i++) {
			tmpnode.clear();
			if (tar[i] >= '0' && tar[i] <= '9') {
				amount = 0; j = i;
				while (j < tarlen && tar[j] >= '0' && tar[j] <= '9')
					amount = amount * 10 + tar[j++] - '0';
				i = j - 1; tmpnode.num = amount; tmpnode.isNum = 1;
				if (i == 0 || (i > 0 && tar[i - 1] == '.'))
					tmpnode.isHead = 1;
				else
					tmpnode.isHead = 0;
			}
			else if ((tar[i] >= 'a' && tar[i] <= 'z') || (tar[i] >= 'A' && tar[i] <= 'Z')) {
				iend = ((i + ELECOL >= tarlen) ? (tarlen - 1) : (i + ELECOL));
				while (iend >= i) {
					eletmp.clear();
					for (j = i; j <= iend; j++)
						eletmp.name += tar[j];
					if (eletmp.Exist())
						break;
					else
						iend--;
				}
				if (iend < i) {
#ifdef TESTCEI
					cout << "遇到无法解析的字符串！在 \"" << tar << "\" 中。" << endl;
#endif
					return 0;
				}
				eletmp.amount = 1;
				tmpnode.block.emplace_back(eletmp);
				tmpnode.isNum = 0;
				i = iend;
			}
			else {//  用num的负数域标记特殊字符
				switch (tar[i]) {
				case '(':tmpnode.num = -7; break;
				case ')':tmpnode.num = -6; break;
				default:
#ifdef TESTCEI
					cout << "发现不合法字符！是 \"" << tar << "\" 中的第"
						<< i + 1 << "个字符 \"" << tar[i] << "\" 。" << endl;
#endif 
					return 0;
				}
				tmpnode.isNum = 1;
			}
			matlist.emplace_back(tmpnode);
		}
		return 1;
	}

	void Matter::furtherLexicalAnalysis(vector<MatNode>& matlist) {
		bool leaveloop = 1;
		int i = 0; int j = 0; int k = 0;
		while (1) {
			for (i = 0; i < (int)matlist.size(); i++)
				if (matlist[i].isNum == 1 && matlist[i].num < 0) {//  如果还存在括号
					leaveloop = 0; break;
				}
			if (leaveloop)	break;	leaveloop = 1;
			for (i = 0; i < (int)matlist.size(); i++) {//  追踪法得到一组可计算括号的位置(j, k)
				if (matlist[i].isNum == 1) {
					if (matlist[i].num == -7)
						j = i;
					else if (matlist[i].num == -6) {
						k = i; break;
					}
				}
			}
			CalaBlock(matlist, j + 1, k - 1);
			matlist.erase(matlist.begin() + j + 1, matlist.begin() + k + 1);
		}
		CallastBlock(matlist);
	}

	bool Matter::CalaBlock(vector<MatNode>& matlist, int i, int j) {
		//  处理一个不含有括号的式子 [i, j]， 数据全部存入matlist[i - 1]
		int k = 0; int head = 1; int coef = 0; int is = i - 1;
		matlist[is].clear();
		if (matlist[i].isNum)				//  注意把前缀数字保留下来最后乘给整个元素整列 否则前缀为1 不影响结果
			head = matlist[i++].num;		//  只处理 matnode[is] 当然后缀只负责更新matnode[k]的数量
		//  开始循环处理 元素阵列的特征   A2 B C3 O ......， 处理完把数据扔到 matlist[i-1] 里面
		//  注意这里的A B C O 本身就是元素阵列 进行循环累加
		while (i <= j) {			//  预设两个matnode为一组 检查第二个是不是数字 否则一个matnode为一组
			coef = 1;
			if (i + 1 <= j && matlist[i + 1].isNum == 1)
				coef = matlist[i + 1].num;
			for (k = 0; k < (int)matlist[i].block.size(); k++) {	//  处理 matlist[i]
				matlist[i].block[k].amount *= (head * coef);		//  修正元素系数
				matlist[is].block.emplace_back(matlist[i].block[k]);	//  把这个Ele挂到阵列后面
			}
			i++;
			while (i <= j && matlist[i].isNum == 1)i++;				//  循环完成以后 matlist[i]是一个字母
		}
		matlist[is].compress();		//  处理完以后压缩一次元素阵列
		return 1;
	}

	bool Matter::CallastBlock(vector<MatNode>& matlist) {
		int i = 0; int j = (int)matlist.size() - 1; int k = 0; int head = 1; int coef = 0;
		MatNode tmp;
		if (matlist[i].isNum)
			head = matlist[i++].num;
		while (i <= j) {
			coef = 1;
			if (i + 1 <= j && matlist[i + 1].isNum == 1)
				coef = matlist[i + 1].num;
			for (k = 0; k < (int)matlist[i].block.size(); k++) {
				matlist[i].block[k].amount *= (head * coef);
				tmp.block.emplace_back(matlist[i].block[k]);
			}
			i++;
			while (i <= j && matlist[i].isNum == 1)i++;
		}
		tmp.compress(); matlist.clear(); matlist.emplace_back(tmp);
		return 1;
	}

	bool Matter::preProcess(string& tar) {
		//  去空格 统一分隔符样式
		deleteAllMark(tar, " ");		replaceAllMark(tar, "·", ".");
		replaceAllMark(tar, "{", "(");	replaceAllMark(tar, "[", "(");
		replaceAllMark(tar, "]", ")");	replaceAllMark(tar, "}", ")");
		int j = 0;
		for (j = 0; j < (int)tar.size(); j++) {
			if (!((tar[j] >= '0' && tar[j] <= '9') || (tar[j] >= 'A' && tar[j] <= 'Z') || (tar[j] >= 'a' && tar[j] <= 'z') || (tar[j] == '.') || (tar[j] == '(') || (tar[j] == ')') || (tar[j] == '@') || (tar[j] == '$')))
				return 0;
		}
		//  修饰并列符“.”
		for (int i = 0; i < (int)tar.length() - 1; i++) {
			if (tar[i] == '.') {
				tar.insert(tar.begin() + i + 1, '(');
				for (j = i + 1; j < (int)tar.length(); j++) {
					if (tar[j] == '.') {
						tar.insert(tar.begin() + j, ')'); break;
					}
				}
				if (j == tar.length())tar.push_back(')');
			}
		}
		deleteAllMark(tar, ".");
		//  去电荷标识符，规定，机内正电荷用@，负电荷用$表示，就近结合
		size_t pos = 0;
		string positive = "@"; string negative = "$";
		q = 0; int cq = 0; int exp = 1;
		while (1) {
			pos = tar.find(positive);
			if (pos == string::npos)break;
			tar.erase(pos, positive.size()); pos--;
			while (pos >= 0 && tar[pos] >= '0' && tar[pos] <= '9') {
				cq = cq + exp * (tar[pos] - '0'); exp *= 10; tar.erase(pos--, 1);
			}
			if (cq == 0)cq = 1;
		}
		q += cq; cq = 0; exp = 1;
		while (1) {
			pos = tar.find(negative);
			if (pos == string::npos)break;
			tar.erase(pos, negative.size()); pos--;
			while (pos >= 0 && tar[pos] >= '0' && tar[pos] <= '9') {
				cq = cq + exp * (tar[pos] - '0'); exp *= 10; tar.erase(pos--, 1);
			}
			if (cq == 0)cq = 1;
		}
		q -= cq;
		deleteAllMark(tar, "@"); deleteAllMark(tar, "$");
		return 1;
	}

	int Matter::getEleAmount(string& _element) {
		for (int i = 0; i < (int)eleList.size(); i++)
			if (_element == eleList[i].name)
				return eleList[i].amount;
		return 0;
	}

	int Matter::getEleIndex(string& _element) {
		for (int i = 0; i < (int)eleList.size(); i++)
			if (_element == eleList[i].name)
				return i;
		return -1;
	}

	string Matter::p() {
		string res = name + stateInfo;
		return res;
	}

	double Matter::getRelativeMolecureMass() {
		int i = 0; double res = 0;
		for (i = 0; i < (int)eleList.size(); i++)
			res += eleList[i].amount * Get_relative_atomic_mass(eleList[i].name);
		return res;
	}

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
		return 1;//确保中间有等号，是个方程
	}

	void Equation::split() {
		int i = 0;
		char* ca = new char[MAXC];
		int index = 0;
		int len = (int)content.length();
		int start = 0;
		while (i < len) {
			if (content[i] < '0' || content[i] > '9') {
				oRatioList.emplace_back(1);
				while (i != len && content[i] != '+' && content[i] != '=')i++;
				i++;
			}
			else {
				start = i; index = 0;
				ca[index++] = content[i++];
				while (content[i] >= '0' && content[i] <= '9')
					ca[index++] = content[i++];
				ca[index] = '\0';
				oRatioList.emplace_back(atoi(ca));
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
		//  检查方程格式
		if (!checkFormat()) {
#ifdef TESTCEI
			cout << "不满足方程的格式：XXX=...=XXX！" << endl;
#endif
			return 0;
		}
		if (content.find("@") != string::npos || content.find("$") != string::npos)
			ie = 1;
		else
			ie = 0;
		split();//  得到去系数方程
		//  处理去系数方程
		int len = (int)equation.length(); int p = 0;
		Matter mat; bool is_reac = 1;
		while (p != len) {
			if (equation[p] == '+') {
				if (is_reac)
					mRList.emplace_back(mat);
				else
					mPList.emplace_back(mat);
				mat.clear();
			}
			else if (equation[p] == '=') {
				if (is_reac)
					mRList.emplace_back(mat);
				else
					mPList.emplace_back(mat);
				mat.clear();
				is_reac = 0;
				while (p != len - 1 && equation[p] == '=')p++;
				p--;
			}
			else if (p == len - 1) {
				mat.name += equation[p];
				if (is_reac)
					mRList.emplace_back(mat);
				else
					mPList.emplace_back(mat);
				mat.clear();
			}
			else
				mat.name += equation[p];
			p++;
		}
		//  元素级拆分
		if (!intoElement()) {
#ifdef TESTCEI
			cout << "字符串拆分失败！未完成元素级拆分。" << endl;
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
			if (addUp && mPList[i].state == 'g')
				mPList[i].stateInfo = "↑";
			else if (addDown && mPList[i].state == 's')
				mPList[i].stateInfo = "↓";
		}
	}


	string Equation::p() {
		char* tmp = new char[MAXC]; string res = ""; i2a(ratioList[0], tmp, 10);
		if (ratioList[0] != 1)res += tmp;
		res += addHtml(mRList[0].name);
		int i = 0; int rs = (int)mRList.size();
		for (i = 1; i < rs; i++) {
			res += " + "; i2a(ratioList[i], tmp, 10);
			if (ratioList[i] != 1)res += tmp;
			res += addHtml(mRList[i].name);
		}
		res += " = "; i2a(ratioList[rs], tmp, 10);
		if (ratioList[rs] != 1)res += tmp;
		res = res + addHtml(mPList[0].name) + mPList[0].stateInfo;
		for (i = 1; i < (int)mPList.size(); i++) {
			res += " + "; i2a(ratioList[i + rs], tmp, 10);
			if (ratioList[i + rs] != 1)res += tmp;
			res = res + addHtml(mPList[i].name) + mPList[i].stateInfo;
		}
		delete[]tmp;
		return res;
	}
	int* Equation::pIndex(int numberOfMolecule) {
		int indexSize = ratioList.size();
		if (numberOfMolecule != indexSize)
			return nullptr;
		int* indexList = new int[indexSize];
		for (size_t i = 0; i < indexSize; i++)
			indexList[i] = ratioList[i];
		return indexList;
	}


	bool Equation::intoElement() {
		//拆分
		for (int i = 0; i < (int)mRList.size(); i++)
			if (!mRList[i].intoElement()) {
#ifdef TESTCEI
				cout << "字串 " << mRList[i].name << " 解析失败！" << endl;
#endif
				return 0;
			}
		for (int i = 0; i < (int)mPList.size(); i++)
			if (!mPList[i].intoElement()) {
#ifdef TESTCEI
				cout << "字串 " << mPList[i].name << " 解析失败！" << endl;
#endif
				return 0;
			}
		//  反应物元素累加、排序、合并
		for (int i = 0; i < (int)mRList.size(); i++) {
			for (int j = 0; j < (int)mRList[i].eleList.size(); j++)
				eRList.emplace_back(mRList[i].eleList[j]);
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
		//  生成物元素累加、排序、合并
		for (int i = 0; i < (int)mPList.size(); i++) {
			for (int j = 0; j < (int)mPList[i].eleList.size(); j++)
				ePList.emplace_back(mPList[i].eleList[j]);
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
			cout << "元素种类不守恒，也许发生了核反应。退出配平！" << endl;
#endif
			return 0;
		}
		int i = 0, j = 0, cr = (int)mRList.size();
		int col = cr + (int)mPList.size();
		int* a = new int[row * col];
		for (i = 0; i < row; i++) {
			for (j = 0; j < cr; j++)
				a[col * i + j] = mRList[j].getEleAmount(eRList[i].name);
			for (j = cr; j < col; j++)
				a[col * i + j] = -mPList[j - cr].getEleAmount(eRList[i].name);
		}
#ifdef TESTCEI
		cout << endl << "元素守恒系数矩阵：" << endl;
		cout << "     o-";
		for (j = 0; j <= col; j++)
			cout << "    ";
		cout << "-o" << endl;
		for (i = 0; i < row; i++) {
			cout << "     |";
			for (j = 0; j < col; j++)
				printf("%4d", a[i * col + j]);
			cout << "      |" << endl;
		}
		cout << "     o_";
		for (j = 0; j <= col; j++)
			cout << "    ";
		cout << "_o" << endl << endl;
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
					redox.emplace_back(zero);
				if (!getRedox()) {
#ifdef TESTCEI
					cout << "无法获得化合价信息以构造方程组，退出配平！" << endl;
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
		cout << endl << "最终系数矩阵：" << endl << "     o-";
		for (j = 0; j <= col; j++)cout << "    ";
		cout << "-o" << endl;
		for (i = 0; i < row; i++) {
			cout << "     |";
			for (j = 0; j < col; j++)printf("%4d", matrix1[i + 1][j + 1]);
			cout << "      |" << endl;
		}
		cout << "     o_";
		for (j = 0; j <= col; j++)cout << "    ";
		cout << "_o" << endl << endl;
#endif
		if (row <= col - 2) {
#ifdef TESTCEI
			cout << "存在多解，需要更多信息，配平失败！" << endl;
#endif
			delete[]a; return 0;
		}

		bool success = Gauss(matrix1, row, col, resint);
		if (!success) {
#ifdef TESTCEI
			cout << "无法进行高斯消元，配平出错！" << endl;
#endif
			delete[]a; return 0;
		}
		for (i = 1; i <= col; i++) {
			if (resint[i] <= 0) {
				//再用随机系数尝试一次
				matrix1[row][1] = 1; matrix1[row][2] = -1;
				for (i = 3; i <= col; i++)
					matrix1[row][i] = 0;
				success = Gauss(matrix1, row, col, resint);
				if (!success) {
#ifdef TESTCEI
					cout << "无法进行高斯消元，配平出错！" << endl;
#endif
					delete[]a; return 0;
				}
				success = 1;
				//进行检查，如果结果合格，强制返回
				for (j = 1; j <= col; j++) {
					if (resint[j] <= 0) {
						success = 0; break;
					}
				}
				if (success) {
					ratioList.clear();
					for (j = 1; j <= col; j++)
						ratioList.emplace_back(resint[j]);
					delete[] a; return 1;
				}
#ifdef TESTCEI
				cout << "高斯消元完成后出现负系数，指定系数依然无效，配平出错！" << endl;
#endif
				delete[]a; return 0;
			}
			ratioList.emplace_back(resint[i]);
		}
		delete[]a;
		return 1;
	}

	bool Equation::getRedox() {
		//获得化合价
		int cr = (int)mRList.size(), cp = (int)mPList.size();
		int i = 0, j = 0, i0 = 0, j0 = 0;
		for (i = 0; i < cr; i++)
			if (!mRList[i].getValence()) {
#ifdef TESTCEI
				cout << "无法获得 " << mRList[i].name << " 的化合价，配平失败！" << endl;
#endif
				return 0;
			}
		for (i = 0; i < cp; i++)
			if (!mPList[i].getValence()) {
#ifdef TESTCEI
				cout << "无法获得 " << mPList[i].name << " 的化合价，配平失败！" << endl;
#endif
				return 0;
			}
		for (i = 0; i < cr; i++) {			//  扫描所有反应物 扫描一个反应物的所有元素
			for (j = 0; j < (int)mRList[i].eleList.size(); j++) {
				for (i0 = 0; i0 < cp; i0++)	//  把该元素的化合价跟产物里面所有该元素的化合价进行比对
					for (j0 = 0; j0 < (int)mPList[i0].eleList.size(); j0++)
						if (mRList[i].eleList[j].name == mPList[i0].eleList[j0].name && mRList[i].eleList[j].valence != mPList[i0].eleList[j0].valence) {
							mRList[i].eleList[j].Reduced = 1; goto BREAKPOINT1;//  第一轮遍历确定哪些元素化合价可能不一样
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

		//  除掉同种元素中化合价没有变化的那部分
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

		//  确定氧化剂和还原剂
		for (i = 0; i < cr; i++) {
			for (j = 0; j < (int)mRList[i].eleList.size(); j++)
				for (i0 = 0; i0 < cp; i0++)
					for (j0 = 0; j0 < (int)mPList[i0].eleList.size(); j0++)
						if (mRList[i].eleList[j].name == mPList[i0].eleList[j0].name && mRList[i].eleList[j].valence != mPList[i0].eleList[j0].valence && mPList[i0].eleList[j0].Reduced == 1)
							if ((Abs(mRList[i].eleList[j].finalValence - mRList[i].eleList[j].valence) > Abs(mPList[i0].eleList[j0].valence - mRList[i].eleList[j].valence)))
								mRList[i].eleList[j].finalValence = mPList[i0].eleList[j0].valence;
		}
		//  写入数据
		Fraction tmp;
		for (i = 0; i < cr; i++) {
			for (j = 0; j < (int)mRList[i].eleList.size(); j++)
				if (mRList[i].eleList[j].Reduced) {//  如果该元素参与了反应
					tmp.a = mRList[i].eleList[j].amount; tmp.b = 1;
					redox[i] = redox[i] + tmp * (mRList[i].eleList[j].valence - mRList[i].eleList[j].finalValence);
				}
		}

#ifdef TESTCEI
		cout << "氧化还原守恒向量：[";
		for (int i = 0; i < cr + cp; i++)
			printf("%4d", redox[i].a);
		cout << "      ]" << endl;
#endif
		return 1;
	}

	bool Equation::IsConversed(vector<int>& raw) {
		int cer = (int)eRList.size(), cep = (int)ePList.size();
		int cr = (int)mRList.size(), cp = (int)mPList.size();
		if (cer != cep) {
			return 0;
		}
		int i = 0, j = 0, k = 0;
		for (i = 0; i < cer; i++) {
			if (eRList[i].name != ePList[i].name)
				return 0;
		}

		int* check = new int[MAXM];
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

	int Equation::getEleIndex(string& tarelenam) {
		for (int i = 0; i < (int)eRList.size(); i++)
			if (tarelenam == eRList[i].name)
				return i;
		return -1;
	}

	bool Equation::exec(string& _content) {
		content = _content;
		if (!intoMatter()) {
#ifdef TESTCEI
			cout << "字符串拆分失败！未完成物质级拆分。" << endl;
#endif
			return 0;
		}
		if (!balance()) {
#ifdef TESTCEI
			cout << "方程式配平失败！" << endl;
#endif
			return 0;
		}
		addStateInfo();
		if (!IsConversed(ratioList)) {
#ifdef TESTCEI
			cout << "机器配平错误！" << endl;
#endif
			return 0;
		}
		cout << p() << endl;
		clear();
		return 1;
	}


	bool Equation::exec() {
		if (!intoMatter()) {
#ifdef TESTCEI
			cout << "字符串拆分失败！未完成物质级拆分。" << endl;
#endif
			return 0;
		}
		if (!balance()) {
#ifdef TESTCEI
			cout << "方程式配平失败！" << endl;
#endif
			return 0;
		}
		addStateInfo();
		if (!IsConversed(ratioList)) {
#ifdef TESTCEI
			cout << "机器配平错误！" << endl;
#endif
			return 0;
		}
		return 1;
	}
};

static chemfunction::Equation equation;

ChemBalancer::ChemBalancer() { ; }

ChemBalancer::~ChemBalancer() { ; }

std::string ChemBalancer::balance(std::string& input) {
	equation.content = input;
	if (!equation.exec()) {
		equation.clear(); return "[ERROR]";
	}
	std::string result = equation.p();
	equation.clear();
	return result;
}

std::string ChemBalancer::addHtml(std::string& input) {
	return chemfunction::addHtml(input);
}
