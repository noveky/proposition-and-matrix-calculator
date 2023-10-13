#pragma once
#include <string>
#include <vector>
using namespace std;

int gcd(int a, int b);

class Rational /// 有理数（分数）
{
public:
	int p = 0;
	int q = 1;

	Rational() { }

	Rational(int p, int q);

	Rational(double r);

	Rational(wstring str);

	Rational(const Rational& x): p(x.p), q(x.q) { }

	void reduce()
	{
		if (q == 0) return;
		if (q < 0) p *= -1, q *= -1;
		int _gcd = gcd(abs(p), abs(q));
		p /= _gcd;
		q /= _gcd;
	}

	double toDouble()
	{
		if (p == 0) return 0.0;
		if (q == 0) return p > 0 ? DBL_MAX : DBL_MIN;
		return double(p) / q;
	}

	wstring toString()
	{
		if (q == 1) return to_wstring(p);
		return to_wstring(p) + L"/" + to_wstring(q);
	}

	Rational& operator++()
	{
		p += q;
		reduce();
		return *this;
	}

	Rational operator++(int)
	{
		Rational tmp(p, q);
		p += q;
		reduce();
		return tmp;
	}

	Rational operator+(const Rational& x2) const
	{
		Rational ret(p * x2.q + q * x2.p, q * x2.q);
		ret.reduce();
		return ret;
	}

	Rational& operator+=(const Rational& x2)
	{
		p = p * x2.q + q * x2.p;
		q *= x2.q;
		reduce();
		return *this;
	}

	Rational operator*(const Rational& x2) const
	{
		Rational ret(p * x2.p, q * x2.q);
		ret.reduce();
		return ret;
	}

	Rational& operator*=(const Rational& x2)
	{
		p *= x2.p;
		q *= x2.q;
		reduce();
		return *this;
	}

	Rational operator-() const
	{
		return *this * -1;
	}

	Rational operator-(const Rational& x2) const
	{
		return *this + -x2;
	}

	Rational operator/(const Rational& x2) const
	{
		Rational ret(p * x2.q, q * x2.p);
		ret.reduce();
		return ret;
	}

	Rational& operator/=(const Rational& x2)
	{
		p *= x2.q;
		q *= x2.p;
		reduce();
		return *this;
	}

	bool operator==(const Rational& x2) const
	{
		Rational tmp1(*this);
		Rational tmp2(x2);
		return tmp1.toDouble() == tmp2.toDouble();
	}

	bool operator!=(const Rational& x2) const
	{
		return !(*this == x2);
	}
};

class Matrix /// 有理数矩阵
{
private:
	vector<vector<Rational>> arr;

public:
	int m = 0, n = 0;

	Matrix() { }

	Matrix(int m, int n) :
		m(m), n(n)
	{
		//arr = new Rational* [m];
		for (int i = 0; i < m; ++i)
		{
			//arr[i] = new Rational[n];
			vector<Rational> tmp;
			for (int j = 0; j < n; ++j)
			{
				tmp.push_back(0);
			}
			arr.push_back(tmp);
		}
	}

	Matrix(const Matrix& M1) : m(M1.m), n(M1.n), arr(M1.arr) { }

	~Matrix()
	{
		/*if (arr == nullptr) return;
		for (int i = 0; i < m; ++i)
		{
			if (arr[i] == nullptr) continue;
			//delete[] arr[i];
		}
		//delete[] arr;*/
	}

	void set(int i, int j, Rational mij)
	{
		if (i <= 0 || j <= 0 || i > m || j > n) return;

		arr[i - 1][j - 1] = mij;
	}

	Rational get(int i, int j)
	{
		if (i <= 0 || j <= 0 || i > m || j > n) return Rational();

		return arr[i - 1][j - 1];
	}

	Rational det(); /// 矩阵的行列式

	Matrix trans(); /// 获得转置矩阵

	Matrix inv(); /// 获得逆矩阵

	void swapRows(int a, int b) { //交换行
		a--;
		b--;
		vector<Rational> temp = arr[a];
		arr[a] = arr[b];
		arr[b] = temp;
	}

	Matrix quickPow(int n); /// 矩阵快速幂

	Matrix gaussianEliminate() { /// 高斯消元法（？）
		Matrix Ab(*this);
		int rows = Ab.m;
		int colums = Ab.n;
		int Acolum = colums - 1;

		int i = 0; /// 跟踪行
		int j = 0; /// 跟踪列
		while (i < rows)
		{
			bool flag = false;
			while (j < Acolum && !flag)
			{
				if (Ab.arr[i][j] != 0)
				{
					flag = true;
				}
				else
				{
					int max_row = i;
					Rational max_val = 0;
					for (int k = i + 1; k < rows; ++k)
					{
						Rational cur_abs = Ab.arr[k][j].toDouble() >= 0 ? Ab.arr[k][j] : Ab.arr[k][j] * -1;
						if (cur_abs.toDouble() > max_val.toDouble())
						{
							max_row = k;
							max_val = cur_abs;
						}
					}
					if (max_row != i)
					{
						Ab.swapRows(max_row, i);
						flag = true;
					}
					else
					{
						j++;
					}
				}
			}
			if (flag)
			{
				for (int t = i + 1; t < rows; t++)
				{
					for (int s = j + 1; s < colums; s++)
					{
						Ab.arr[t][s] = Ab.arr[t][s] - Ab.arr[i][s] * (Ab.arr[t][j] / Ab.arr[i][j]);
						if (abs(Ab.arr[t][s].toDouble()) < 1e-10)
							Ab.arr[t][s] = 0;
					}
					Ab.arr[t][j] = 0;
				}
			}
			i++;
			j++;
		}
		return Ab;
	}

	Matrix operator+(const Matrix& m1)
	{
		if (m1.m != m || m1.n != n) return Matrix();

		Matrix ret(m, n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				ret.set(i + 1, j + 1, arr[i][j] + m1.arr[i][j]);
			}
		}
		return ret;
	}

	Matrix operator*(const Rational& k) const
	{
		Matrix ret(m, n);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				ret.set(i + 1, j + 1, arr[i][j] * k);
			}
		}
		return ret;
	}

	Matrix operator-(const Matrix& m1)
	{
		return *this + m1 * -1;
	}

	Matrix operator*(const Matrix& m1)
	{
		if (n != m1.m) return Matrix();

		int p = m1.n;
		Matrix ret(m, p);
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < p; ++j)
			{
				Rational cij = 0;
				for (int k = 0; k < n; ++k)
				{
					cij += arr[i][k] * m1.arr[k][j];
				}
				ret.set(i + 1, j + 1, cij);
			}
		}
		return ret;
	}
};