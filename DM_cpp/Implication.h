#pragma once
#include <string>
#include <memory>
#include "Function.h"
using namespace std;

class Implication : public Function
{
public:
	shared_ptr<Function> F; /// Operand 1
	shared_ptr<Function> G; /// Operand 2

	Implication(shared_ptr<Function> F, shared_ptr<Function> G);

	wstring toString();

	bool getValue()
	{
		return !F->getValue() || G->getValue();
	}

	shared_ptr<Function> toNand();

	shared_ptr<Function> toNor();
};
