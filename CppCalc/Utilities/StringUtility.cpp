#pragma once
#include <string>
#include <vector>
using namespace std;

namespace StringUtility
{
	/*void trim(string& str)
	{
		int i = 0;
		while (i < int(str.size()) && str[i] == ' ') ++i;
		str.erase(0, i);

		int j = str.size() - 1;
		while (j >= 0 && str[j] == ' ') --j;
		str.erase(j + 1, str.size() - j - 1);
	}*/

	void trim(wstring& str)
	{
		int i = 0;
		while (i < int(str.size()) && str[i] == L' ') ++i;
		str.erase(0, i);

		int j = str.size() - 1;
		while (j >= 0 && str[j] == L' ') --j;
		str.erase(j + 1, str.size() - j - 1);
	}

	/*vector<string> split(const string& str, char ch)
	{
		vector<string> ret;
		int i = 0;
		int j = 0;
		while (true)
		{
			j = str.find(ch, i);
			if (j == -1) break;
			ret.push_back(str.substr(i, j - i));
			i = j + 1;
		}
		ret.push_back(str.substr(i, str.size() - i));
		return ret;
	}*/

	vector<wstring> split(const wstring& str, wchar_t ch)
	{
		vector<wstring> ret;
		int i = 0;
		int j = 0;
		while (true)
		{
			j = str.find(ch, i);
			if (j == -1) break;
			ret.push_back(str.substr(i, j - i));
			i = j + 1;
		}
		ret.push_back(str.substr(i, str.size() - i));
		return ret;
	}
}