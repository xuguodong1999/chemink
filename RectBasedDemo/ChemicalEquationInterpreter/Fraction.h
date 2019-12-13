#ifndef FRACTION_H
#define FRACTION_H
#include"config.h"
class Fraction {
public:
	Fraction();
	Fraction(int x);
	~Fraction();
	Fraction operator = (int x);
	Fraction operator = (const Fraction x);
	Fraction operator + (const Fraction x);
	Fraction operator - (const Fraction x);
	Fraction operator * (const Fraction x);
	Fraction operator / (const Fraction x);
	bool operator < (const Fraction x);
	bool operator <= (const Fraction x);
	bool operator > (const Fraction x);
	bool operator >= (const Fraction x);
	bool operator == (const Fraction x);
	bool operator != (const Fraction x);
	void a2f(char* tar);
	void a2f(std::string& tar);
	std::string p();
	friend Fraction s2Frac(char* tar);
	friend Fraction s2Frac(const std::string& tar);
	friend Fraction Abs(Fraction x);
	friend bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]);
	friend void Matrix_row_transformation(int m[][MAXM], int& row, int& col);
	int a; int b;
	void reduce();
};
int gcd(int x, int y);
int lcm(int x, int y);
bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]);
void Matrix_row_transformation(int m[][MAXM], int& row, int& col);
Fraction s2Frac(char* tar);
Fraction s2Frac(const std::string& tar);
Fraction Abs(Fraction x);
#endif
