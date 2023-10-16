#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Matrix.h" /// ���þ���ͷ�ļ�
#include "Interface.h" /// ���ý���ͷ�ļ�
#include "MyString.h"
using namespace std;
using namespace Interface;

namespace MatrixCalc
{
	LOGFONT font;
	LOGFONT font1;
	int m1, n1, m2, n2;
	shared_ptr<Form> form;
	shared_ptr<Label> label1;
	shared_ptr<Label> label2;
	shared_ptr<Label> label3;
	shared_ptr<Label> label4;
	shared_ptr<Label> label5;
	shared_ptr<Label> label6;
	shared_ptr<Label> label7;
	shared_ptr<TextBox> textBox1;
	shared_ptr<TextBox> textBox2;
	shared_ptr<TextBox> textBox3;
	shared_ptr<TextBox> textBox4;
	shared_ptr<TextBox> textBox5;
	shared_ptr<TextBox> textBox6;
	shared_ptr<TextBox> textBox7;
	Button* button1;
	Button* button2;
	Button* button3;
	Button* button4;
	Button* button5;
	Button* button6;
	Button* button7;
	Button* button8;
	Button* button9;
	const int w = 56, h = 32, spc = 16;
	vector<shared_ptr<TextBox>> txtA;
	vector<shared_ptr<TextBox>> txtB;
	vector<shared_ptr<TextBox>> txtC;

	int read(wstring str)
	{
		MyString::trim(str);
		if (str.length() == 0) return 0;
		int i = 0;
		int x = 0, f = 1;
		wchar_t ch = str[i++];
		while (ch < L'0' || ch > L'9') { if (ch == L'-') f = -1; ch = str[i++]; }
		while (ch >= L'0' && ch <= L'9') { x = (x << 3) + (x << 1) + ch - L'0'; ch = str[i++]; }
		return x * f;
	}

	void button1_Click()
	{
		m1 = MatrixCalc::read(textBox1->text);
		n1 = MatrixCalc::read(textBox2->text);
		m2 = MatrixCalc::read(textBox3->text);
		n2 = MatrixCalc::read(textBox4->text);
		if (m1 == 0) textBox1->text = L"0";
		if (n1 == 0) textBox2->text = L"0";
		if (m2 == 0) textBox3->text = L"0";
		if (n2 == 0) textBox4->text = L"0";

		for (shared_ptr<TextBox> txt : txtA)
		{
			form->RemoveControl(*txt);
		}
		txtA.clear();
		for (shared_ptr<TextBox> txt : txtB)
		{
			form->RemoveControl(*txt);
		}
		txtB.clear();
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();
		
		for (int i = 0; i < m1 * n1; ++i)
		{
			int line = i / n1, row = i % n1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				20 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, false, false);
			txtA.push_back(txt);
		}
		for (int i = 0; i < m2 * n2; ++i)
		{
			int line = i / n2, row = i % n2;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				340 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, false, false);
			txtB.push_back(txt);
		}
	}

	void button2_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 != m2 || n1 != n2)
		{
			form->MsgBox(L"��ͬ�;����޷���ӣ�", L"������������");
			return;
		}
		
		int m = m1, n = n1;

		Matrix A(m1, n1), B(m2, n2), C(m, n);

		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				Rational bij(txtB[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				if (bij == 0) txtB[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
				B.set(i + 1, j + 1, bij);
			}
		}

		C = A + B;

		for (int i = 0; i < m * n; ++i)
		{
			int line = i / n, row = i % n;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button3_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 != m2 || n1 != n2)
		{
			form->MsgBox(L"��ͬ�;����޷������", L"������������");
			return;
		}

		int m = m1, n = n1;

		Matrix A(m1, n1), B(m2, n2), C(m, n);

		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				Rational bij(txtB[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				if (bij == 0) txtB[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
				B.set(i + 1, j + 1, bij);
			}
		}

		C = A - B;

		for (int i = 0; i < m * n; ++i)
		{
			int line = i / n, row = i % n;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button4_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (n1 != m2)
		{
			form->MsgBox(L"��������������������������", L"������������");
			return;
		}

		int m = m1, n = n1, p = n2;

		Matrix A(m1, n1), B(m2, n2), C(m, p);

		for (int i = 0; i < m1; ++i)
		{
			for (int j = 0; j < n1; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}
		for (int i = 0; i < m2; ++i)
		{
			for (int j = 0; j < n2; ++j)
			{
				int ind = i * n2 + j;
				Rational bij(txtB[ind]->text);
				if (bij == 0) txtB[ind]->text = L"0";
				B.set(i + 1, j + 1, bij);
			}
		}

		C = A * B;

		for (int i = 0; i < m * p; ++i)
		{
			int line = i / p, row = i % p;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button5_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 != n1)
		{
			form->MsgBox(L"�Ƿ��������ʽ�����ڣ�", L"������������");
			return;
		}

		if (m1 == 0 || n1 == 0) return;

		int n = m1;

		Matrix A(n, n), C(1, 1);

		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				int ind = i * n + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}

		C.set(1, 1, A.det());

		for (int i = 0; i < 1; ++i)
		{
			int line = i / 1, row = i % 1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button6_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 == 0 || n1 == 0) return;

		Matrix A(m1, n1), C(n1, m1);

		for (int i = 0; i < m1; ++i)
		{
			for (int j = 0; j < n1; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}

		C = A.trans();

		for (int i = 0; i < n1 * m1; ++i)
		{
			int line = i / m1, row = i % m1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button7_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 != n1)
		{
			form->MsgBox(L"�Ƿ��������󲻴��ڣ�", L"������������");
			return;
		}

		if (m1 == 0 || n1 == 0) return;

		Matrix A(m1, n1), C(n1, m1);

		for (int i = 0; i < m1; ++i)
		{
			for (int j = 0; j < n1; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}

		if (A.det() == 0)
		{
			form->MsgBox(L"A���ǿ������", L"������������");
			return;
		}

		C = A.inv();

		for (int i = 0; i < n1 * m1; ++i)
		{
			int line = i / m1, row = i % m1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button8_Click()
	{
		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 == 0 || n1 == 0) return;

		Matrix A(m1, n1), C(m1, n1);

		for (int i = 0; i < m1; ++i)
		{
			for (int j = 0; j < n1; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}

		C = A.gaussianEliminate();

		for (int i = 0; i < m1 * n1; ++i)
		{
			int line = i / n1, row = i % n1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void button9_Click()
	{
		if (m2 == 0 || n2 == 0)
		{
			form->MsgBox(L"����BΪ�գ��޷�ȡ��b11��", L"������������");
			return;
		}

		if (m1 != n1)
		{
			form->MsgBox(L"�Ƿ����޷����г˷����㣡", L"������������");
			return;
		}

		for (shared_ptr<TextBox> txt : txtC)
		{
			form->RemoveControl(*txt);
		}
		txtC.clear();

		if (m1 == 0 || n1 == 0) return;

		Matrix A(m1, n1), C(m1, n1);

		for (int i = 0; i < m1; ++i)
		{
			for (int j = 0; j < n1; ++j)
			{
				int ind = i * n1 + j;
				Rational aij(txtA[ind]->text);
				if (aij == 0) txtA[ind]->text = L"0";
				A.set(i + 1, j + 1, aij);
			}
		}

		Rational b11(txtB[0]->text);
		if (b11 == 0) txtB[0]->text = L"0";

		if (b11.q != 1 || b11.p < 0)
		{
			form->MsgBox(L"b11���ǷǸ�������", L"������������");
			return;
		}
		int t = b11.p;
		C = A.quickPow(t);

		for (int i = 0; i < m1 * n1; ++i)
		{
			int line = i / n1, row = i % n1;
			shared_ptr<TextBox> txt = make_shared<TextBox>(*form,
				660 + (w + spc) * row,
				20 + (32 + 16) * 6.5 + (h + spc) * line,
				w, h, &font, true, false);
			txt->text = C.get(line + 1, row + 1).toString();
			txtC.push_back(txt);
		}
	}

	void textBoxUpdate(shared_ptr<TextBox> textBox)
	{
		using namespace MatrixCalc;
		wstring newText = L"";
		for (wchar_t& ch : textBox->text)
		{
			if (ch >= L'0' && ch <= L'9') newText += ch;
		}
		textBox->text = newText;
	}

	void textBox1_Update()
	{
		textBoxUpdate(textBox1);
	}

	void textBox2_Update()
	{
		textBoxUpdate(textBox1);
	}

	void textBox3_Update()
	{
		textBoxUpdate(textBox1);
	}

	void textBox4_Update()
	{
		textBoxUpdate(textBox1);
	}
}


void MatrixCalculator()
{
	using namespace MatrixCalc;
	form = make_shared<Form>(1000, 600, L"������������");
	font.lfWeight = FW_DONTCARE;
	font.lfOutPrecision = OUT_DEFAULT_PRECIS;
	font.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	font.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	font.lfCharSet = DEFAULT_CHARSET;
	_tcscpy_s(font.lfFaceName, _T("Monaco"));
	font.lfQuality = DEFAULT_QUALITY;
	font.lfWidth = 0;
	font.lfHeight = 22;
	font.lfOrientation = 0;
	font.lfEscapement = 0;
	font.lfStrikeOut = false;
	font.lfUnderline = false;
	font.lfItalic = false;
	font1 = font;
	_tcscpy_s(font1.lfFaceName, _T("΢���ź�"));

	label1 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 0, L"����A��������", &font1);
	label2 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 1, L"����A��������", &font1);
	label3 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 2, L"����B��������", &font1);
	label4 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 3, L"����B��������", &font1);
	textBox1 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 0, 80, 32, &font, false, false, textBox1_Update);
	textBox2 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 1, 80, 32, &font, false, false, textBox2_Update);
	textBox3 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 2, 80, 32, &font, false, false, textBox2_Update);
	textBox4 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 3, 80, 32, &font, false, false, textBox2_Update);
	//TODO shared_ptr
	button1 = new Button(*form, 20, 20 + (32 + 16) * 4, 160, 36, &font1, L"ȷ��", nullptr, button1_Click);
	textBox1->defButton = textBox2->defButton = textBox3->defButton = textBox4->defButton = button1;

	button2 = new Button(*form, 340, 20 + (32 + 16) * 0, 160, 36, &font1, L"A��B", nullptr, button2_Click);
	button3 = new Button(*form, 340, 20 + (32 + 16) * 1, 160, 36, &font1, L"A��B", nullptr, button3_Click);
	button4 = new Button(*form, 340, 20 + (32 + 16) * 2, 160, 36, &font1, L"AB", nullptr, button4_Click);
	button9 = new Button(*form, 340, 20 + (32 + 16) * 3, 160, 36, &font1, L"A^b11", nullptr, button9_Click);
	button5 = new Button(*form, 340 + 160 + 36, 20 + (32 + 16) * 0, 160, 36, &font1, L"detA", nullptr, button5_Click);
	button6 = new Button(*form, 340 + 160 + 36, 20 + (32 + 16) * 1, 160, 36, &font1, L"A^T", nullptr, button6_Click);
	button7 = new Button(*form, 340 + 160 + 36, 20 + (32 + 16) * 2, 160, 36, &font1, L"A^-1", nullptr, button7_Click);
	button8 = new Button(*form, 340 + 160 + 36, 20 + (32 + 16) * 3, 160, 36, &font1, L"A�Ľ��ݻ�", nullptr, button8_Click);

	label5 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 5.5, L"�������A��", &font1);
	label6 = make_shared<Label>(*form, 340, 25 + (32 + 16) * 5.5, L"�������B��", &font1);
	label7 = make_shared<Label>(*form, 660, 25 + (32 + 16) * 5.5, L"��������", &font1);
	//Rational x1(-16.5), x2(L"106/5");
	//x1 /= x2;
	//textBox1->text = L"(~a&~b&~c&d)|(~a&~b&c&d)|(~a&b&~c&d)|(~a&b&c&d)|(a&~b&c&~d)|(a&~b&c&d)|(a&b&c&~d)";
	//textBox4->text = x1.toString() + L'\n' + x2.toString() + L'\n' + to_wstring(x1.toDouble());
	form->Main();
}

//int main()
//{
	//DMCalculator();
	/*for (; true; cout << endl) /// ѭ������
	{
		cout << "F = ";
		string expr; /// ����һ���ַ��������������û�����ı��ʽ
		getline(cin, expr); /// ������ʽ
		shared_ptr<Function> F;
		try
		{
			F = getFunction("F = " + expr); /// ���ݱ��ʽ�������⹫ʽF

			/// ����ı��ʽ�Ϸ�
			cout << "F = " << F->toString() << endl; /// ������⹫ʽF�ı��ʽ��Ӧ�ú�������ʽ�ǻ���һ�µ�ʽ�ӣ�����ȥ����������ţ�
			cout << "Truth table: \n" << F->getTruthTable().toString(); /// ������⹫ʽF����ֵ��
			cout << "Principal disjunctive normal form: \n" << F->getPrincipalDisjunctive().toString() << " = " << F->getPrincipalDisjunctive().toFunction()->toString() << endl; /// ������⹫ʽF������ȡ��ʽ
			cout << "Principal conjunctive normal form: \n" << F->getPrincipalConjunctive().toString() << " = " << F->getPrincipalConjunctive().toFunction()->toString() << endl; /// ������⹫ʽF������ȡ��ʽ
			cout << "Simplest form: \n" << F->toSimplest()->toString() << endl; /// ������⹫ʽF�������ʽ
			cout << "NAND form: \n" << F->toSimplest()->toNand()->toString() << endl; /// ������⹫ʽF�������ʽ
			cout << "NOR form: \n" << F->toSimplest()->toNor()->toString() << endl; /// ������⹫ʽF�Ļ����ʽ
		}
		catch (...)
		{
			cout << "Invalid expression!" << endl; /// ����ı��ʽ���Ϸ�����ʾ�����ʽ���Ϸ���
		}
	}*/

	//return 0;
//}

/*
TruthTable magic()
{
	cout << "F = ";
	string expr; /// ����һ���ַ��������������û�����ı��ʽ
	getline(cin, expr); /// ������ʽ
	shared_ptr<Function> F;
	TruthTable truthTable;
	try
	{
		F = getFunction("F = " + expr); /// ���ݱ��ʽ�������⹫ʽF

		/// ����ı��ʽ�Ϸ�
		cout << "F = " << F->toString() << endl; /// ������⹫ʽF�ı��ʽ��Ӧ�ú�������ʽ�ǻ���һ�µ�ʽ�ӣ�����ȥ����������ţ�
		truthTable = F->getTruthTable();
		cout << "Truth table: \n" << truthTable.toString(); /// ������⹫ʽF����ֵ��
		//cout << "Truth table: \n" << F->getTruthTable().toString(); /// ������⹫ʽF����ֵ��
		cout << "Principal disjunctive normal form: \n" << F->getDisjunctive()->toString() << endl; /// ������⹫ʽF������ȡ��ʽ
		cout << "Principal conjunctive normal form: \n" << F->getConjunctive()->toString() << endl; /// ������⹫ʽF������ȡ��ʽ
	}
	catch (...)
	{
		cout << "Invalid expression!" << endl; /// ����ı��ʽ���Ϸ�����ʾ�����ʽ���Ϸ���}
	}
	return truthTable;
}

int main()
{
	for (; true; cout << endl) /// ѭ������
	{
		TruthTable t = magic();
		cout << t.toString();
	}

	return 0;
}
*/