#pragma once
#include <string>
#include <memory>
#include "Conjunction.h"
#include "Negation.h"
#include "Nand.h"
#include "Nor.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Conjunction::Conjunction(shared_ptr<Function> F, shared_ptr<Function> G)
{
	this->F = F;
	this->G = G;

	/// Initialize variables
	variables = mergeVariables(F->variables, G->variables);

	/// Operator
	opt = CONJUNCTION;

	/// Operands' brackets
	F->brackets = F->getOpt() != opt && (F->getPriority() == UNDEFINED_PRIORITY || F->getPriority() < getPriority());
	G->brackets = G->getOpt() != opt && (G->getPriority() == UNDEFINED_PRIORITY || G->getPriority() < getPriority());
}

wstring Conjunction::toString()
{
	return (brackets ? L"(" : L"") + F->toString() + L" " + getOptSymbol(opt) + L" " + G->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Conjunction::toNand()
{
	return make_shared<Negation>(make_shared<Nand>(F->toNand(), G->toNand()))->toNand();
}

shared_ptr<Function> Conjunction::toNor()
{
	return make_shared<Nor>(make_shared<Negation>(F)->toNor(), make_shared<Negation>(G)->toNor());
}