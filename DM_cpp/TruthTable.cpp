#pragma once
#include <string>
#include <memory>
#include "TruthTable.h"
#include "Function.h"
#include "Variable.h"
using namespace std;

wstring TruthTable::toString()
{
	wstring str;

	/// Header
	for (int i = 0; i < int(function->variables.size()); ++i)
	{
		str += function->variables[i]->getSymbol();
		str += L"   ";
	}
	str += function->getSymbol();
	str += L'\n';

	/// Body
	for (int i = 0; i < int(tableVector.size()); ++i)
	{
		int variableCnt = int(function->variables.size());
		for (int j = 0; j < variableCnt; ++j)
		{
			str += to_wstring((i >> (variableCnt - j - 1)) & 1);
			str += L"   ";
		}
		str += to_wstring(tableVector[i]);
		str += L'\n';
	}
	return str;
}