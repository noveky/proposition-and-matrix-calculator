#pragma once
#include <string>
#include <memory>
#include "Nand.h"
#include "Disjunction.h"
#include "Negation.h"
using namespace std;

wstring getOptSymbol(int opt);
int getOptPriority(int opt, bool input = false);
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2);

Nand::Nand(shared_ptr<Function> F, shared_ptr<Function> G)
{
	this->F = F;
	this->G = G;

	/// Initialize variables
	variables = mergeVariables(F->variables, G->variables);

	/// Operator
	opt = NAND;

	/// Operands' brackets
	F->brackets = F->getPriority() < MAX_PRIORITY;
	G->brackets = G->getPriority() < MAX_PRIORITY;
}

wstring Nand::toString()
{
	return (brackets ? L"(" : L"") + F->toString() + L" " + getOptSymbol(opt) + L" " + G->toString() + (brackets ? L")" : L"");
}

shared_ptr<Function> Nand::toNand()
{
	return make_shared<Nand>(F->toNand(), G->toNand());
}

shared_ptr<Function> Nand::toNor()
{
	return make_shared<Disjunction>(make_shared<Negation>(F->toNor()), make_shared<Negation>(G->toNor()))->toNor();
}