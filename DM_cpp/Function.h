#pragma once
#include <vector>
#include <set>
#include <string>
#include <stack>
#include <algorithm>
#include <memory>
#include "Variable.h"
using namespace std;

/// Enumeration type for operator and their priorities
enum Operator { VARIABLE_AS_FUNCTION, CONSTANT_AS_FUNCTION, NEGATION, CONJUNCTION, DISJUNCTION, NAND, NOR, IMPLICATION, LEFT_BRACKET, RIGHT_BRACKET };
enum OptPriority { UNDEFINED_PRIORITY = INT_MIN, MAX_PRIORITY = 0 , MIN_PRIORITY = INT_MIN + 1};

class TruthTable;
class Variable;
class DisjunctiveNormal;
class ConjunctiveNormal;

class Function
{
protected:
	/// Indicator of the operator of the function; uses enumeration type Operator
	int opt = -1;

	/// Symbol of the function
	wchar_t symbol = L'F';

public:
	/// Whether to add brackets when converting to string
	bool brackets = false;

	/// A vector of related variables in the function
	vector<shared_ptr<Variable>> variables;

	/// Gets a variable pointer from variables according to a specific symbol
	shared_ptr<Variable> getVariable(wchar_t symbol);

	/// Default constructor
	Function() { }

	int getOpt()
	{
		return opt;
	}

	wchar_t getSymbol()
	{
		return symbol;
	}

	void setSymbol(wchar_t symbol)
	{
		this->symbol = symbol;
	}

	virtual wstring toString()
	{
		return wstring();
	}

	/// Gets the final value of the function
	virtual bool getValue()
	{
		return false;
	}

	/// Gets the priority of the operation
	int getPriority();

	/// Assigns a group of values to variables according to a binary array and get the value
	bool assign(int binArray);

	/// Gets truth table
	TruthTable getTruthTable();

	/// Gets principal disjunctive normal form
	DisjunctiveNormal getPrincipalDisjunctive();

	/// Gets principal conjunctive normal form
	ConjunctiveNormal getPrincipalConjunctive();

	/// Gets simplest form
	shared_ptr<Function> toSimplest();

	virtual shared_ptr<Function> toNand()
	{
		return make_shared<Function>();
	}

	virtual shared_ptr<Function> toNor()
	{
		return make_shared<Function>();
	}
};
