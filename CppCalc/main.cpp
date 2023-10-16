#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "Function.h" /// ����Function�����⹫ʽ�����ͷ�ļ�
#include "Matrix.h" /// ���þ���ͷ�ļ�
#include "Interface.h" /// ���ý���ͷ�ļ�
using namespace std;
using namespace Interface;

namespace Main
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
}

void DMCalculator();
void MatrixCalculator();
int main()
{
	using namespace Main;
	form = make_shared<Form>(640, 480, L"�๦�ܼ�����");
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

	Button button1(*form, 20, 20, 180, 36, &font1, L"���⹫ʽ������", nullptr, DMCalculator);
	Button button2(*form, 20, 20 + 36 + 16, 180, 36, &font1, L"������������", nullptr, MatrixCalculator);
	form->Main();

	return 0;
}