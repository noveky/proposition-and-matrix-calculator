#pragma once
#include <string>
#include <memory>
#include "Function.h"
using namespace std;

class Conjunction : public Function
{
public:
	shared_ptr<Function> F; /// Operand 1
	shared_ptr<Function> G; /// Operand 2

	Conjunction(shared_ptr<Function> F, shared_ptr<Function> G);

	wstring toString();

	bool getValue()
	{
		return F->getValue() && G->getValue();
	}

	shared_ptr<Function> toNand();

	shared_ptr<Function> toNor();
};
