#pragma once
#include <string>
#include <memory>
#include "Disjunction.h"
#include "Nand.h"
#include "Negation.h"
#include "Nor.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Disjunction::Disjunction(shared_ptr<Function> F, shared_ptr<Function> G)
{
	this->F = F;
	this->G = G;

	/// Initialize variables
	variables = mergeVariables(F->variables, G->variables);

	/// Operator
	opt = DISJUNCTION;

	/// Operands' brackets
	F->brackets = F->getOpt() != opt && (F->getPriority() == UNDEFINED_PRIORITY || F->getPriority() < getPriority());
	G->brackets = G->getOpt() != opt && (G->getPriority() == UNDEFINED_PRIORITY || G->getPriority() < getPriority());
}

wstring Disjunction::toString()
{
	return (brackets ? L"(" : L"") + F->toString() + L" " + getOptSymbol(opt) + L" " + G->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Disjunction::toNand()
{
	return make_shared<Nand>(make_shared<Negation>(F)->toNand(), make_shared<Negation>(G)->toNand());
}

shared_ptr<Function> Disjunction::toNor()
{
	return make_shared<Negation>(make_shared<Nor>(F->toNor(), G->toNor()))->toNor();
}