#pragma once
#include <vector>
#include <set>
#include <algorithm>
#include <memory>
#include "Variable.h"
using namespace std;

int Variable::nxtIndex = 0;

/// A compare function for shared_ptr<Variable>, in order to sort containers of type shared_ptr<Variable>
static bool compVariablePtr(shared_ptr<Variable> vp1, shared_ptr<Variable> vp2)
{
	return *vp1 < *vp2;
}

/// Merge two variable vectors and sort
vector<shared_ptr<Variable>> mergeVariables(vector<shared_ptr<Variable>> variables1, vector<shared_ptr<Variable>> variables2)
{
	vector<shared_ptr<Variable>> variables;

	variables.insert(variables.end(), variables1.begin(), variables1.end());
	variables.insert(variables.end(), variables2.begin(), variables2.end());

	set<shared_ptr<Variable>> variableSet(variables.begin(), variables.end());
	variables.assign(variableSet.begin(), variableSet.end());

	sort(variables.begin(), variables.end(), compVariablePtr);

	return variables;
}