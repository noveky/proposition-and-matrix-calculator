#pragma once
#include <memory>
#include "Function.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

class ConstantAsFunction : public Function
{
private:
	bool value = false;

public:
	ConstantAsFunction(bool value)
	{
		this->value = value;

		/// Initialize variables

		/// Operator
		opt = CONSTANT_AS_FUNCTION;
	}

	wstring toString()
	{
		return (brackets ? L"(" : L"") + to_wstring(int(value)) + (brackets ? L")" : L"");
	}

	bool getValue()
	{
		return value;
	}

	shared_ptr<Function> toNand()
	{
		return make_shared<ConstantAsFunction>(*this);
	}

	shared_ptr<Function> toNor()
	{
		return make_shared<ConstantAsFunction>(*this);
	}
};
