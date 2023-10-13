#pragma once
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Function;

/// Should be disposed of once the function is deleted
class TruthTable
{
private:
	Function* function = nullptr;
	vector<bool> tableVector;

public:
	TruthTable(Function& functionRef, vector<bool> tableVector)
	{
		this->function = &functionRef;
		this->tableVector = tableVector;
	}

	wstring toString();
};
