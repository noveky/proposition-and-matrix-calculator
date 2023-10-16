#pragma once
#include <string>
#include <memory>
#include "Implication.h"
#include "Disjunction.h"
#include "Negation.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Implication::Implication(shared_ptr<Function> F, shared_ptr<Function> G)
{
	this->F = F;
	this->G = G;

	/// Initialize variables
	variables = mergeVariables(F->variables, G->variables);

	/// Operator
	opt = IMPLICATION;
	
	/// Operands' brackets
	F->brackets = F->getPriority() == UNDEFINED_PRIORITY || F->getPriority() < getPriority();
	G->brackets = G->getPriority() == UNDEFINED_PRIORITY || G->getPriority() < getPriority();
}

wstring Implication::toString()
{
	return (brackets ? L"(" : L"") + F->toString() + L" " + getOptSymbol(opt) + L" " + G->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Implication::toNand()
{
	return make_shared<Disjunction>(make_shared<Negation>(F->toNand()), G->toNand())->toNand();
}

shared_ptr<Function> Implication::toNor()
{
	return make_shared<Disjunction>(make_shared<Negation>(F->toNor()), G->toNor())->toNor();
}