#include "Fraction.h"
#include "Function.h"
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
	return a * x.b > b * x.a;
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
void Fraction::a2f(std::string& tar) {
	a2f((char*)tar.c_str());
}
std::string Fraction::p() {
	std::string fout;
	char *tmpa = new char[MAXC];
	char *tmpb = new char[MAXC];
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
	return x % y == 0 ? y : gcd(y, x%y);
}
int lcm(int x, int y) {
	return x * y / gcd(x, y);
}
Fraction s2Frac(char* tar) {
	Fraction res;
	char*a = new char[MAXC];
	char*b = new char[MAXC];
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
Fraction s2Frac(const std::string& tar) {
	return s2Frac((char*)tar.c_str());
}
Fraction Abs(Fraction x) {
	int p = x.a > 0 ? x.a : -x.a, q = x.b > 0 ? x.b : -x.b;
	Fraction tmp;
	tmp.a = p; tmp.b = q;
	return tmp;
}
bool Gauss(int imatrix[][MAXM], int row, int col, int ires[]) {
	Fraction fres[MAXM];
	Fraction fmatrix[MAXM][MAXM];
	fres[col] = 1;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			fmatrix[i][j] = imatrix[i][j];
	for (int i = 1; i <= row; i++)
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
	}
	for (int i = 1; i <= col - 1; i++)  fres[i] = fmatrix[i][N + 1];
	int tmp2 = lcm(fres[1].b, fres[2].b);
	for (int i = 3; i <= col; i++)  tmp2 = lcm(tmp2, fres[i].b);
	for (int i = 1; i <= col; i++)  ires[i] = fres[i].a * tmp2 / fres[i].b;
	return true;
}
void Matrix_row_transformation(int m[][MAXM], int& row, int& col) {
	int i = 0, j = 0, k = 0; Fraction ratio = 0, tmp = 0; bool ok = 0, first_not_zero = 0;
	for (i = 0; i <= row - 1; i++) {
		for (j = i + 1; j <= row; j++) {
			ok = 0; ratio = 0; first_not_zero = 1;
			for (k = 1; k <= col; k++) {
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
				else {
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

