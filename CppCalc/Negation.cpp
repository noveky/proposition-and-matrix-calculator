#pragma once
#include <string>
#include <memory>
#include "Negation.h"
#include "Nand.h"
#include "Nor.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Negation::Negation(shared_ptr<Function> F)
{
	this->F = F;

	/// Initialize variables
	variables = F->variables;

	/// Operator
	opt = NEGATION;

	/// Operands' brackets
	F->brackets = F->getPriority() == UNDEFINED_PRIORITY || F->getPriority() < getPriority();
}

wstring Negation::toString()
{
	return (brackets ? L"(" : L"") + getOptSymbol(opt) + F->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Negation::toNand()
{
	return make_shared<Nand>(F->toNand(), F->toNand());
}

shared_ptr<Function> Negation::toNor()
{
	return make_shared<Nor>(F->toNor(), F->toNor());
}