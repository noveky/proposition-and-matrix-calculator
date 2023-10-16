#pragma once
#include <string>
#include <vector>
using namespace std;

namespace StringUtility
{
	//void trim(string& str);

	void trim(wstring& str);

	//vector<string> split(const string& str, char ch);

	vector<wstring> split(const wstring& str, wchar_t ch);
}