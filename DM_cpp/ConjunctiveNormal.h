#pragma once
#include <vector>
#include <memory>
#include "Function.h"
#include "Variable.h"
#include "VariableAsFunction.h"
#include "ConstantAsFunction.h"
#include "Negation.h"
#include "Conjunction.h"
#include "Disjunction.h"
using namespace std;

class ConjunctiveNormal
{
private:
	Function* function = nullptr;
	vector<int> maxterms;
	bool isConstant = false; /// If this is false, the function may also be a tautology or contradiction!
	bool constantValue = false;

public:
	/// Constructor for *principal* conjunctive normal form of the function
	ConjunctiveNormal(Function& functionRef)
	{
		this->function = &functionRef;

		int variableCnt = int(function->variables.size());
		if (variableCnt == 0)
		{
			isConstant = true;
			constantValue = function->getValue();
			return;
		}

		int maxtermCnt = 0;
		for (int binArray = 0; binArray < (1 << variableCnt); ++binArray)
		{
			if (!function->assign(binArray))
			{
				maxterms.push_back(binArray);
				++maxtermCnt;
			}
		}
		if (maxtermCnt == 0)
		{
			isConstant = true;
			constantValue = true;
		}
	}

	wstring toString()
	{
		if (isConstant) return to_wstring(int(constantValue));

		wstring ret = L"¦°M(";
		int maxtermCnt = maxterms.size();
		for (int i = 0; i < maxtermCnt; ++i)
		{
			ret += to_wstring(maxterms[i]) + (i == maxtermCnt - 1 ? L")" : L",");
		}
		return ret;
	}

	shared_ptr<Function> toFunction()
	{
		if (isConstant) return make_shared<ConstantAsFunction>(constantValue);

		shared_ptr<Function> ret(make_shared<Function>());
		int variableCnt = function->variables.size();
		for (int i = 0; i < int(maxterms.size()); ++i)
		{
			int binArray = maxterms[i];
			shared_ptr<Function> maxterm(make_shared<Function>());
			for (int j = 0; j < variableCnt; ++j)
			{
				shared_ptr<Function> v(make_shared<VariableAsFunction>(function->variables[j]));
				if (((binArray >> (variableCnt - j - 1)) & 1) == 1)
				{
					shared_ptr<Function> vn(make_shared<Negation>(v));
					v = vn;
				}
				if (j == 0) maxterm = v;
				else
				{
					maxterm = make_shared<Disjunction>(maxterm, v);
				}
			}
			if (i == 0) ret = maxterm;
			else
			{
				ret = make_shared<Conjunction>(ret, maxterm);
			}
		}
		return ret;
	}
};
