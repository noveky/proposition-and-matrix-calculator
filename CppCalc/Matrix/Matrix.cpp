#include "Matrix.h"
#include "../Utilities/StringUtility.h"

int gcd(int a, int b)
{
	if (b == 0) return 0;
	int r = a % b;
	if (r == 0) return b;
	return gcd(b, r);
}

double read(wstring str, int& i, int& decDigits /* 小数位数 */)
{
	StringUtility::trim(str);
	if (str.length() == 0) return 0;
	double x = 0, f = 1;
	double t = 0;
	wchar_t ch = str[i++];
	decDigits = 0;
	while (ch < L'0' || ch > L'9') { if (ch == L'-') f *= -1; ch = str[i++]; }
	while (ch >= L'0' && ch <= L'9') { x = x * 10 + (ch - L'0'); ch = str[i++]; }
	if (ch == L'.') { ch = str[i++]; t = 1; }
	while (t != 0 && ch >= L'0' && ch <= L'9') { t /= 10; x += t * (ch - L'0'); ++decDigits;  ch = str[i++]; }
	return x * f;
}

Rational::Rational(int p, int q) : p(p), q(q)
{
	reduce();
}

Rational::Rational(double r)
{
	for (p = 1, q = 1; abs(r * q * 10) < INT_MAX && double(p) / double(q) != r; q *= 10, p = int(r * q));
	reduce();
}

Rational::Rational(wstring str)
{
	StringUtility::trim(str);
	if (str.length() == 0) return;
	int i = 0;
	int decDigits_p = 0, decDigits_q = 0; /// 小数位数
	double p = read(str, i, decDigits_p);
	double q = this->q;
	if (str[i - 1] == L'/')
	{
		q = read(str, i, decDigits_q);
	}
	int decDigits = max(decDigits_p, decDigits_q);
	while (decDigits--)
	{
		p *= 10;
		q *= 10;
	}
	this->p = int(p);
	this->q = int(q);
	reduce();
}

vector<vector<Rational>> getComplementMinor(vector<vector<Rational> > det, int i, int j) { ///获得余子式（不是代数余子式）
	int n = det.size();
	vector<vector<Rational> > ans(n - 1);
	for (int k = 0; k < n - 1; k++)
	{
		for (int l = 0; l < n - 1; l++)
		{
			ans[k].push_back(det[k < i ? k : k + 1][l < j ? l : l + 1]);
		}
	}
	return ans;
}

Rational getDetVal(vector<vector<Rational> > det) /// 获得行列式的值
{
	int m = det.size();
	if (m == 0) return Rational();
	int n = det[0].size();
	if (m != n) return Rational();

	if (n == 1) return det[0][0];

	Rational ans = 0;
	for (int i = 0; i < n; i++)
	{
		ans += det[0][i] * pow(-1, i) * getDetVal(getComplementMinor(det, 0, i));
	}
	return ans;
}

Rational Matrix::det()
{
	if (m != n) return Rational();

	return getDetVal(arr);
}

Matrix Matrix::trans()
{
	Matrix k(n, m);
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			k.set(j + 1, i + 1, arr[i][j]);
		}
	}
	return k;
}

Matrix Matrix::inv()
{
	Rational D = getDetVal(arr);
	if (m != n || D == 0) return Matrix();

	Matrix k(m, n);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < m; j++) {
			Rational q = m == 1 ? Rational(1) / D : getDetVal(getComplementMinor(arr, i, j)) / D;
			if ((i + j) % 2 == 1) q = -q;
			k.set(i + 1, j + 1, q);
		}
	}
	return k.trans();
}

Matrix operator*(const Rational& k, Matrix& m)
{
	return m * k;
}

Matrix Matrix::quickPow(int t) { /// 矩阵快速幂
	if (m != n) return Matrix();

	Matrix res(m, m);
	for (int i = 0; i < m; ++i)
	{
		res.set(i + 1, i + 1, 1);
	}

	Matrix p(*this);
	while (t) {
		if (t & 1) res = res = res * p;
		p = p * p;
		t /= 2;
	}
	return res;
}
