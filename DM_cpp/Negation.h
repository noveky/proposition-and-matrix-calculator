#pragma once
#include <string>
#include <memory>
#include "Function.h"
using namespace std;

class Negation : public Function
{
public:
	shared_ptr<Function> F; /// Operand
	
	Negation(shared_ptr<Function> F);

	wstring toString();

	bool getValue()
	{
		return !F->getValue();
	}

	shared_ptr<Function> toNand();

	shared_ptr<Function> toNor();
};
