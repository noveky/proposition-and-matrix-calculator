#pragma once
#include <string>
#include <memory>
using namespace std;

class Variable
{
private:
	bool value = false;
	wchar_t symbol = L'\0';
	int index = -1;
	static int nxtIndex; /// A static integer, which stores the index for the next Variable instance

public:
    Variable(wchar_t symbol)
    {
		this->symbol = symbol;

		/// Initialize index
		index = nxtIndex++;
    }

	Variable() { }

	bool operator<(const Variable& v1)
	{
		return index < v1.index;
	}
	
	wchar_t getSymbol()
	{
		return symbol;
	}

	void setValue(bool value)
	{
		this->value = value;
	}

    wstring toString()
    {
		wstring s(1, symbol);
		return s;
    }

	bool getValue()
	{
		return value;
	}
};
