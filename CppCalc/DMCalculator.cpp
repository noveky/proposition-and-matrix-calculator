#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Function.h" /// 引用Function（命题公式）类的头文件
#include "TruthTable.h" /// 引用TruthTable（真值表）类的头文件
#include "DisjunctiveNormal.h" /// 引用DisjunctiveNormal（析取范式）类的头文件
#include "ConjunctiveNormal.h" /// 引用ConjunctiveNormal（合取范式）类的头文件
#include "Matrix.h" /// 引用矩阵头文件
#include "Interface.h" /// 引用界面头文件
using namespace std;
using namespace Interface;

shared_ptr<Function> getFunction(wstring str); /// 前向声明，位于Function.cpp中的函数，功能是根据字符串表达式str生成命题公式对象

namespace DMCalc
{
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

	void button1_Click()
	{
		using namespace DMCalc;
		try
		{
			shared_ptr<Function> F = getFunction(L"F = " + textBox1->text);
			textBox2->text = F->getPrincipalDisjunctive().toString() + L"\n<=>" + F->getPrincipalDisjunctive().toFunction()->toString();
			textBox3->text = F->getPrincipalConjunctive().toString() + L"\n<=>" + F->getPrincipalConjunctive().toFunction()->toString();
			textBox4->text = F->toSimplest()->toNand()->toString();
			textBox5->text = F->toSimplest()->toNor()->toString();
			textBox6->text = F->toSimplest()->toString();
			textBox7->text = F->getTruthTable().toString();
		}
		catch (...)
		{
			form->MsgBox(L"表达式不合法！", L"命题公式计算器");
		}
	}

	void textBox1_Update()
	{
		using namespace DMCalc;
		/// 自动取代表达式中的替代符号
		for (wchar_t& ch : textBox1->text)
		{
			if (ch == L'!') ch = L'┐';
			if (ch == L'~') ch = L'┐';
			if (ch == L'&') ch = L'∧';
			if (ch == L'|') ch = L'∨';
			if (ch == L'^') ch = L'↑';
			if (ch == L'_') ch = L'↓';
			if (ch == L'>') ch = L'→';
		}
	}
}

void DMCalculator()
{
	using namespace DMCalc;
	form = make_shared<Form>(1280, 680, L"命题公式计算器");
	LOGFONT font;
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
	LOGFONT font1 = font;
	_tcscpy_s(font1.lfFaceName, _T("微软雅黑"));

	label1 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 0, L"表达式：F = ", &font1);
	label2 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 1.5, L"主析取范式：", &font1);
	label3 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 3.5, L"主合取范式：", &font1);
	label4 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 5.5, L"与非形式：", &font1);
	label5 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 6.5, L"或非形式：", &font1);
	label6 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 7.5, L"最简展开式：", &font1);
	textBox1 = make_shared<TextBox>(*form, 120, 20 + (32 + 16) * 0, 1240 - 120 + 20, 32, &font, false, false, textBox1_Update);
	textBox2 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 1.5, 800 - 135 + 20, 32 * 2 + 16, &font, true, true);
	textBox3 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 3.5, 800 - 135 + 20, 32 * 2 + 16, &font, true, true);
	textBox4 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 5.5, 800 - 135 + 20, 32, &font, true, false);
	textBox5 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 6.5, 800 - 135 + 20, 32, &font, true, false);
	textBox6 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 7.5, 800 - 135 + 20, 32, &font, true, false);
	label7 = make_shared<Label>(*form, 860, 25 + (32 + 16) * 1.5, L"真值表：", &font1);
	textBox7 = make_shared<TextBox>(*form, 860, 20 + (32 + 16) * 2.3, 400, 32 * 16, &font, true, true);
	Button button1(*form, 20, 20 + (32 + 16) * 8.7, 120, 36, &font1, L"分析", nullptr, button1_Click);
	textBox1->defButton = textBox2->defButton = textBox3->defButton = textBox4->defButton = &button1;
	//Rational x1(-16.5), x2(L"106/5");
	//x1 /= x2;
	//textBox1->text = L"(~a&~b&~c&d)|(~a&~b&c&d)|(~a&b&~c&d)|(~a&b&c&d)|(a&~b&c&~d)|(a&~b&c&d)|(a&b&c&~d)";
	//textBox4->text = x1.toString() + L'\n' + x2.toString() + L'\n' + to_wstring(x1.toDouble());
	form->Main();
}

//int main()
//{
	//DMCalculator();
	/*for (; true; cout << endl) /// 循环输入
	{
		cout << "F = ";
		string expr; /// 定义一个字符串，用来接受用户输入的表达式
		getline(cin, expr); /// 输入表达式
		shared_ptr<Function> F;
		try
		{
			F = getFunction("F = " + expr); /// 根据表达式构建命题公式F

			/// 输入的表达式合法
			cout << "F = " << F->toString() << endl; /// 输出命题公式F的表达式（应该和输入表达式是基本一致的式子，但会去掉多余的括号）
			cout << "Truth table: \n" << F->getTruthTable().toString(); /// 输出命题公式F的真值表
			cout << "Principal disjunctive normal form: \n" << F->getPrincipalDisjunctive().toString() << " = " << F->getPrincipalDisjunctive().toFunction()->toString() << endl; /// 输出命题公式F的主析取范式
			cout << "Principal conjunctive normal form: \n" << F->getPrincipalConjunctive().toString() << " = " << F->getPrincipalConjunctive().toFunction()->toString() << endl; /// 输出命题公式F的主合取范式
			cout << "Simplest form: \n" << F->toSimplest()->toString() << endl; /// 输出命题公式F的最简形式
			cout << "NAND form: \n" << F->toSimplest()->toNand()->toString() << endl; /// 输出命题公式F的与非形式
			cout << "NOR form: \n" << F->toSimplest()->toNor()->toString() << endl; /// 输出命题公式F的或非形式
		}
		catch (...)
		{
			cout << "Invalid expression!" << endl; /// 输入的表达式不合法，提示“表达式不合法”
		}
	}*/

	//return 0;
//}

/*
TruthTable magic()
{
	cout << "F = ";
	string expr; /// 定义一个字符串，用来接受用户输入的表达式
	getline(cin, expr); /// 输入表达式
	shared_ptr<Function> F;
	TruthTable truthTable;
	try
	{
		F = getFunction("F = " + expr); /// 根据表达式构建命题公式F

		/// 输入的表达式合法
		cout << "F = " << F->toString() << endl; /// 输出命题公式F的表达式（应该和输入表达式是基本一致的式子，但会去掉多余的括号）
		truthTable = F->getTruthTable();
		cout << "Truth table: \n" << truthTable.toString(); /// 输出命题公式F的真值表
		//cout << "Truth table: \n" << F->getTruthTable().toString(); /// 输出命题公式F的真值表
		cout << "Principal disjunctive normal form: \n" << F->getDisjunctive()->toString() << endl; /// 输出命题公式F的主析取范式
		cout << "Principal conjunctive normal form: \n" << F->getConjunctive()->toString() << endl; /// 输出命题公式F的主合取范式
	}
	catch (...)
	{
		cout << "Invalid expression!" << endl; /// 输入的表达式不合法，提示“表达式不合法”}
	}
	return truthTable;
}

int main()
{
	for (; true; cout << endl) /// 循环输入
	{
		TruthTable t = magic();
		cout << t.toString();
	}
	
	return 0;
}
*/