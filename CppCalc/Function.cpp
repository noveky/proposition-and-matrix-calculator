#pragma once
#include <iostream>
#include <memory>
#include "Variable.h"
#include "VariableAsFunction.h"
#include "Negation.h"
#include "Conjunction.h"
#include "Disjunction.h"
#include "Nand.h"
#include "Nor.h"
#include "Implication.h"
#include "ConstantAsFunction.h"
#include "Function.h"
#include "TruthTable.h"
#include "MyString.h"
#include "DisjunctiveNormal.h"
#include "ConjunctiveNormal.h"
using namespace std;

int getOptPriority(int opt, bool input = false);

shared_ptr<Variable> Function::getVariable(wchar_t symbol)
{
	for (vector<shared_ptr<Variable>>::iterator p = variables.begin(); p != variables.end(); ++p)
	{
		if ((*p)->getSymbol() == symbol)
		{
			return *p;
		}
	}
	throw 0;
}

int Function::getPriority()
{
	return getOptPriority(opt);
}

bool Function::assign(int binArray)
{
	int variableCnt = int(variables.size());
	for (int i = 0; i < variableCnt; ++i)
	{
		variables[variableCnt - i - 1]->setValue(((binArray >> i) & 1) == 1);
	}
	return getValue();
}

TruthTable Function::getTruthTable()
{
	vector<bool> tableVector;
	for (int binArray = 0; binArray < (1 << variables.size()); ++binArray)
	{
		tableVector.push_back(assign(binArray));
	}
	TruthTable truthTable(*this, tableVector); /// This "make_shared" only takes the members of Function out of the current instance, because it uses the copy constructor of Function, the base class
	return truthTable;
}

/*
shared_ptr<Function> Function::getDisjunctive()
{
	int variableCnt = int(variables.size());
	if (variableCnt == 0) return make_shared<ConstantAsFunction>(getValue());
	shared_ptr<Function> ret(make_shared<Function>());
	int mintermCnt = 0;
	for (int binArray = 0; binArray < (1 << variableCnt); ++binArray)
	{
		if (assign(binArray))
		{
			shared_ptr<Function> minterm(make_shared<Function>());
			for (int i = 0; i < variableCnt; ++i)
			{
				shared_ptr<Function> v(make_shared<VariableAsFunction>(variables[i]));
				if (((binArray >> (variableCnt - i - 1)) & 1) == 0)
				{
					shared_ptr<Function> vn(make_shared<Negation>(v));
					v = vn;
				}
				if (i == 0) minterm = v;
				else
				{
					minterm = make_shared<Conjunction>(minterm, v);
				}
			}
			if (mintermCnt == 0) ret = minterm;
			else
			{
				ret = make_shared<Disjunction>(ret, minterm);
			}
			++mintermCnt;
		}
	}
	if (mintermCnt == 0) ret = make_shared<ConstantAsFunction>(false);
	return ret;
}

shared_ptr<Function> Function::getConjunctive()
{
	int variableCnt = int(variables.size());
	if (variableCnt == 0) return make_shared<ConstantAsFunction>(getValue());
	shared_ptr<Function> ret(make_shared<Function>());
	int maxtermCnt = 0;
	for (int binArray = 0; binArray < (1 << variableCnt); ++binArray)
	{
		if (!assign(binArray))
		{
			shared_ptr<Function> maxterm(make_shared<Function>());
			for (int i = 0; i < variableCnt; ++i)
			{
				shared_ptr<Function> v(make_shared<VariableAsFunction>(variables[i]));
				if (((binArray >> (variableCnt - i - 1)) & 1) == 1)
				{
					shared_ptr<Function> vn(make_shared<Negation>(v));
					v = vn;
				}
				if (i == 0) maxterm = v;
				else
				{
					maxterm = make_shared<Disjunction>(maxterm, v);
				}
			}
			if (maxtermCnt == 0) ret = maxterm;
			else
			{
				ret = make_shared<Conjunction>(ret, maxterm);
			}
			++maxtermCnt;
		}
	}
	if (maxtermCnt == 0) ret = make_shared<ConstantAsFunction>(true);
	return ret;
}
*/

DisjunctiveNormal Function::getPrincipalDisjunctive()
{
	DisjunctiveNormal principalDisjunctive(*this);
	return principalDisjunctive;
}

ConjunctiveNormal Function::getPrincipalConjunctive()
{
	ConjunctiveNormal principalConjunctive(*this);
	return principalConjunctive;
}

shared_ptr<Function> Function::toSimplest()
{
	return getPrincipalDisjunctive().simplify();
}

/// Gets operator according to its symbol
static int getOperator(wchar_t symbol)
{
	switch (symbol)
	{
	case L'0':
	case L'1':
		return CONSTANT_AS_FUNCTION;
	case L'┐':
	case L'!':
	case L'~':
		return NEGATION;
	case L'∧':
	case L'&':
		return CONJUNCTION;
	case L'∨':
	case L'|':
		return DISJUNCTION;
	case L'↑':
	case L'^':
		return NAND;
	case L'↓':
	case L'_':
		return NOR;
	case L'→':
	case L'>':
		return IMPLICATION;
	case L'(':
		return LEFT_BRACKET;
	case L')':
		return RIGHT_BRACKET;
	default:
		if (symbol >= L'a' && symbol <= L'z') return VARIABLE_AS_FUNCTION;
		return -1;
	}
}

wstring getOptSymbol(int opt)
{
	switch (opt)
	{
	case NEGATION:
		return L"┐";//"~";
	case CONJUNCTION:
		return L"∧";//"&";
	case DISJUNCTION:
		return L"∨";//"|";
	case NAND:
		return L"↑";//"^";
	case NOR:
		return L"↓";//"_";
	case IMPLICATION:
		return L"→";
	default:
		return wstring();
	}
}

/// Get the priority of an operator
int getOptPriority(int opt, bool input)
{
	switch (opt)
	{
	case VARIABLE_AS_FUNCTION:
	case CONSTANT_AS_FUNCTION:
		return MAX_PRIORITY;
	case NEGATION:
		return -1;
	case CONJUNCTION:
		return input ? -3 : UNDEFINED_PRIORITY;//-3;
	case DISJUNCTION:
		return input ? -4 : UNDEFINED_PRIORITY;//-4;
	case IMPLICATION:
		return input ? -5 : MIN_PRIORITY;//-5;
	case NAND:
	case NOR:
		return UNDEFINED_PRIORITY;
	default:
		return UNDEFINED_PRIORITY;
	}
}

/// Get operator priority from its symbol
static int getOptPriority(wchar_t symbol, bool input = false)
{
	return getOptPriority(getOperator(symbol), input);
}

shared_ptr<Function> getFunction(wstring fullExpr)
{
	/// Split the full expression (e.g. "F = p&~q") into function symbol (e.g. 'F') and expression (e.g. "p&~q")
	vector<wstring> splitExpr = MyString::split(fullExpr, L'=');
	if (splitExpr.size() != 2) throw 0;
	wstring func = splitExpr[0];
	MyString::trim(func);
	if (func.length() != 1) throw 0;
	wchar_t funcSymbol = func[0];
	if (!(funcSymbol >= L'A' && funcSymbol <= L'Z')) throw 0;
	wstring expr = splitExpr[1];
	MyString::trim(expr);

	struct ExprUnit
	{
		bool isFunction = false;
		shared_ptr<Function> F = make_shared<Function>();
		int opt = -1;
		ExprUnit(shared_ptr<Function> F) { this->F = F; isFunction = true; }
		ExprUnit(int opt) { this->opt = opt; }
	};
	stack<int> optStack;
	vector<ExprUnit> postExpr;
	Function tmpFunction;
	
	for (int i = 0; i < int(expr.length()); ++i)
	{
		wchar_t c = expr[i];
		int opt = getOperator(c);
		switch (opt)
		{
		case VARIABLE_AS_FUNCTION:
		{
			shared_ptr<Variable> variable;
			try
			{
				variable = tmpFunction.getVariable(c);
			}
			catch (...)
			{
				shared_ptr<Variable> e(make_shared<Variable>(c));
				tmpFunction.variables.push_back(variable = e);
			}
			shared_ptr<Function> variableAsFunction(make_shared<VariableAsFunction>(variable));
			ExprUnit exprUnit(variableAsFunction);
			postExpr.push_back(exprUnit);
			break;
		}
		case CONSTANT_AS_FUNCTION:
		{
			shared_ptr<Function> constantAsFunction(make_shared<ConstantAsFunction>(c == L'1'));
			ExprUnit exprUnit(constantAsFunction);
			postExpr.push_back(exprUnit);
			break;
		}
		case NEGATION:
		{
			while (!optStack.empty() && optStack.top() != LEFT_BRACKET && getOptPriority(c, true) < getOptPriority(optStack.top(), true))
			{
				postExpr.push_back(optStack.top());
				optStack.pop();
			}
			optStack.push(getOperator(c));
			break;
		}
		case CONJUNCTION:
		case DISJUNCTION:
		case NAND:
		case NOR:
		case IMPLICATION:
		{
			while (!optStack.empty() && optStack.top() != LEFT_BRACKET && getOptPriority(c, true) <= getOptPriority(optStack.top(), true))
			{
				postExpr.push_back(optStack.top());
				optStack.pop();
			}
			optStack.push(getOperator(c));
			break;
		}
		case LEFT_BRACKET:
		{
			optStack.push(getOperator(c));
			break;
		}
		case RIGHT_BRACKET:
		{
			while (!optStack.empty() && optStack.top() != LEFT_BRACKET)
			{
				postExpr.push_back(optStack.top());
				optStack.pop();
			}
			if (optStack.empty()) throw 0;
			if (optStack.top() == LEFT_BRACKET)
			{
				optStack.pop();
			}
			break;
		}
		default:
		{
			if (c == L' ' || c == L'\t' || c == L'\r' || c == L'\n') continue;
			throw 0;
		}
		}
	}
	while (!optStack.empty())
	{
		if (optStack.top() == LEFT_BRACKET) throw 0;
		postExpr.push_back(optStack.top());
		optStack.pop();
	}

	/*/// Test
	cout << "Postfix notation: ";
	for (vector<ExprUnit>::iterator p = postExpr.begin(); p != postExpr.end(); ++p)
	{
		cout << (p->isFunction ? p->F->toString() : getOptSymbol(p->opt));
	}
	cout << endl;*/

	stack<shared_ptr<Function>> fStack;
	for (vector<ExprUnit>::iterator p = postExpr.begin(); p != postExpr.end(); ++p)
	{
		if (p->isFunction)
		{
			fStack.push(p->F);
		}
		else
		{
			switch (p->opt)
			{
			case NEGATION:
			{
				if (fStack.empty()) throw 0;
				shared_ptr<Function> F = fStack.top();
				fStack.pop();
				shared_ptr<Function> T(make_shared<Negation>(F));
				fStack.push(T);
				break;
			}
			case CONJUNCTION:
			case DISJUNCTION:
			case NAND:
			case NOR:
			case IMPLICATION:
			{
				if (fStack.empty()) throw 0;
				shared_ptr<Function> F = fStack.top();
				fStack.pop();
				if (fStack.empty()) throw 0;
				shared_ptr<Function> G = fStack.top();
				fStack.pop();
				shared_ptr<Function> T; 
				switch (p->opt)
				{
				case CONJUNCTION:
					T = make_shared<Conjunction>(G, F);
					break;
				case DISJUNCTION:
					T = make_shared<Disjunction>(G, F);
					break;
				case NAND:
					T = make_shared<Nand>(G, F);
					break;
				case NOR:
					T = make_shared<Nor>(G, F);
					break;
				case IMPLICATION:
					T = make_shared<Implication>(G, F);
					break;
				}
				fStack.push(T);
				break;
			}
			default:
				throw 0;
			}
		}
	}
	if (fStack.size() != 1) throw 0;
	fStack.top()->setSymbol(funcSymbol);
	return fStack.top();
}
