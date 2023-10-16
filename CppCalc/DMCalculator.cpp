#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Function.h" /// ����Function�����⹫ʽ�����ͷ�ļ�
#include "TruthTable.h" /// ����TruthTable����ֵ�����ͷ�ļ�
#include "DisjunctiveNormal.h" /// ����DisjunctiveNormal����ȡ��ʽ�����ͷ�ļ�
#include "ConjunctiveNormal.h" /// ����ConjunctiveNormal����ȡ��ʽ�����ͷ�ļ�
#include "Matrix.h" /// ���þ���ͷ�ļ�
#include "Interface.h" /// ���ý���ͷ�ļ�
using namespace std;
using namespace Interface;

shared_ptr<Function> getFunction(wstring str); /// ǰ��������λ��Function.cpp�еĺ����������Ǹ����ַ������ʽstr�������⹫ʽ����

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
			form->MsgBox(L"���ʽ���Ϸ���", L"���⹫ʽ������");
		}
	}

	void textBox1_Update()
	{
		using namespace DMCalc;
		/// �Զ�ȡ�����ʽ�е��������
		for (wchar_t& ch : textBox1->text)
		{
			if (ch == L'!') ch = L'��';
			if (ch == L'~') ch = L'��';
			if (ch == L'&') ch = L'��';
			if (ch == L'|') ch = L'��';
			if (ch == L'^') ch = L'��';
			if (ch == L'_') ch = L'��';
			if (ch == L'>') ch = L'��';
		}
	}
}

void DMCalculator()
{
	using namespace DMCalc;
	form = make_shared<Form>(1280, 680, L"���⹫ʽ������");
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
	_tcscpy_s(font1.lfFaceName, _T("΢���ź�"));

	label1 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 0, L"���ʽ��F = ", &font1);
	label2 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 1.5, L"����ȡ��ʽ��", &font1);
	label3 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 3.5, L"����ȡ��ʽ��", &font1);
	label4 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 5.5, L"�����ʽ��", &font1);
	label5 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 6.5, L"�����ʽ��", &font1);
	label6 = make_shared<Label>(*form, 20, 25 + (32 + 16) * 7.5, L"���չ��ʽ��", &font1);
	textBox1 = make_shared<TextBox>(*form, 120, 20 + (32 + 16) * 0, 1240 - 120 + 20, 32, &font, false, false, textBox1_Update);
	textBox2 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 1.5, 800 - 135 + 20, 32 * 2 + 16, &font, true, true);
	textBox3 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 3.5, 800 - 135 + 20, 32 * 2 + 16, &font, true, true);
	textBox4 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 5.5, 800 - 135 + 20, 32, &font, true, false);
	textBox5 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 6.5, 800 - 135 + 20, 32, &font, true, false);
	textBox6 = make_shared<TextBox>(*form, 135, 20 + (32 + 16) * 7.5, 800 - 135 + 20, 32, &font, true, false);
	label7 = make_shared<Label>(*form, 860, 25 + (32 + 16) * 1.5, L"��ֵ��", &font1);
	textBox7 = make_shared<TextBox>(*form, 860, 20 + (32 + 16) * 2.3, 400, 32 * 16, &font, true, true);
	Button button1(*form, 20, 20 + (32 + 16) * 8.7, 120, 36, &font1, L"����", nullptr, button1_Click);
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