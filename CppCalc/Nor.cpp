#pragma once
#include <string>
#include <memory>
#include "Nor.h"
#include "Conjunction.h"
#include "Negation.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Nor::Nor(shared_ptr<Function> F, shared_ptr<Function> G)
{
	this->F = F;
	this->G = G;

	/// Initialize variables
	variables = mergeVariables(F->variables, G->variables);

	/// Operator
	opt = NOR;

	/// Operands' brackets
	F->brackets = F->getPriority() < MAX_PRIORITY;
	G->brackets = G->getPriority() < MAX_PRIORITY;
}

wstring Nor::toString()
{
	return (brackets ? L"(" : L"") + F->toString() + L" " + getOptSymbol(opt) + L" " + G->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Nor::toNand()
{
	return make_shared<Conjunction>(make_shared<Negation>(F), make_shared<Negation>(G))->toNand();
}

shared_ptr<Function> Nor::toNor()
{
	return make_shared<Nor>(F->toNor(), G->toNor());
}