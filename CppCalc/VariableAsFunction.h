#pragma once
#include <memory>
#include<iostream>
#include "Function.h"
#include "Variable.h"
using namespace std;

class VariableAsFunction : public Function
{
public:
	shared_ptr<Variable> V; /// The variable

	VariableAsFunction(shared_ptr<Variable> V)
	{
		this->V = V;

		/// Initialize variables
		variables.push_back(V);

		/// Operator
		opt = VARIABLE_AS_FUNCTION;
	}

	wstring toString()
	{
		return (brackets ? L"(" : L"") + wstring(1, V->getSymbol()) + (brackets ? L")" : L"");
	}

	bool getValue()
	{
		return V->getValue();
	}

	shared_ptr<Function> toNand()
	{
		return make_shared<VariableAsFunction>(*this);
	}

	shared_ptr<Function> toNor()
	{
		return make_shared<VariableAsFunction>(*this);
	}
};
