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

class DisjunctiveNormal
{
private:
	Function* function = nullptr;
	vector<int> minterms;
	bool isConstant = false; /// If this is false, the function may also be a tautology or contradiction!
	bool constantValue = false;

public:
	/// Constructor for *principal* disjunctive normal form of the function
	DisjunctiveNormal(Function& functionRef)
	{
		this->function = &functionRef;

		int variableCnt = int(function->variables.size());
		if (variableCnt == 0)
		{
			isConstant = true;
			constantValue = function->getValue();
			return;
		}

		int mintermCnt = 0;
		for (int binArray = 0; binArray < (1 << variableCnt); ++binArray)
		{
			if (function->assign(binArray))
			{
				minterms.push_back(binArray);
				++mintermCnt;
			}
		}
		if (mintermCnt == 0)
		{
			isConstant = true;
			constantValue = false;
		}
	}

	wstring toString()
	{
		if (isConstant) return to_wstring(int(constantValue));

		wstring ret = L"Σm(";
		int mintermCnt = minterms.size();
		for (int i = 0; i < mintermCnt; ++i)
		{
			ret += to_wstring(minterms[i]) + (i == mintermCnt - 1 ? L")" : L",");
		}
		return ret;
	}

	shared_ptr<Function> toFunction()
	{
		if (isConstant) return make_shared<ConstantAsFunction>(constantValue);

		shared_ptr<Function> ret(make_shared<Function>());
		int variableCnt = function->variables.size();
		for (int i = 0; i < int(minterms.size()); ++i)
		{
			int binArray = minterms[i];
			shared_ptr<Function> minterm(make_shared<Function>());
			for (int j = 0; j < variableCnt; ++j)
			{
				shared_ptr<Function> v(make_shared<VariableAsFunction>(function->variables[j]));
				if (((binArray >> (variableCnt - j - 1)) & 1) == 0)
				{
					shared_ptr<Function> vn(make_shared<Negation>(v));
					v = vn;
				}
				if (j == 0) minterm = v;
				else
				{
					minterm = make_shared<Conjunction>(minterm, v);
				}
			}
			if (i == 0) ret = minterm;
			else
			{
				ret = make_shared<Disjunction>(ret, minterm);
			}
		}
		return ret;
	}

	shared_ptr<Function> simplify() /// 根据主析取范式化简
	{
		if (isConstant) return make_shared<ConstantAsFunction>(constantValue);
		
		int variableCnt = function->variables.size();

		/// 获取析取范式的所有项
		vector<vector<int>> terms;
		vector<set<int>> coverage; /// 记录每个项覆盖的极小项
		for (int minterm : minterms)
		{
			vector<int> vec;
			set<int> cov;
			for (int i = 0, t = minterm; i < variableCnt; ++i, t >>= 1)
			{
				vec.push_back(t & 1);
			}
			cov.emplace(minterm);

			terms.push_back(vec);
			coverage.push_back(cov);
		}

		/// 合并相邻项直到无法合并为止
		while (true)
		{
			bool merging = false; /// 根据是否进行了合并操作，标记是否仍在进行合并
			vector<vector<int>> newTerms;
			vector<set<int>> newCoverage;
			vector<bool> merged(terms.size(), false); /// 标记每个项是否被合并了
			for (int i = 0; i < int(terms.size()); ++i)
			{
				for (int j = i + 1; j < int(terms.size()); ++j)
				{
					int diff = 0, diffDigit = -1;
					for (int k = 0; k < variableCnt; ++k)
					{
						if (terms[i][k] != terms[j][k])
						{
							++diff;
							if (terms[i][k] != -1 && terms[j][k] != -1) diffDigit = k;
						}
					}
					if (diff == 1 && diffDigit != -1)
					{
						/// 两个项只有一位不同，合并（用-1表示）
						vector<int> mergedTerm = terms[i];
						mergedTerm[diffDigit] = -1;
						newTerms.push_back(mergedTerm);

						/// 合并两项所覆盖的极小项集合
						set<int> mergedCoverage = coverage[i];
						mergedCoverage.insert(coverage[j].begin(), coverage[j].end());
						newCoverage.push_back(mergedCoverage);

						/// 更新标记
						merging = true;
						merged[i] = merged[j] = true;
					}
				}
			}

			if (!merging) break;
			/// 将未合并的项原封不动补充到newTerms和newCoverage
			for (int i = 0; i < int(terms.size()); ++i)
			{
				if (!merged[i])
				{
					newTerms.push_back(terms[i]);
					newCoverage.push_back(coverage[i]);
				}
			}
			terms = newTerms;
			coverage = newCoverage;
		}
		/*while (true)
		{
			bool merge = false; /// 标记是否进行了合并操作
			vector<vector<int>> newTerms = terms;
			for (int i = 0; i < int(terms.size()); ++i)
			{
				for (int j = i + 1; j < int(terms.size()); ++j)
				{
					int diff = 0, diffDigit = -1;
					for (int k = 0; k < variableCnt; ++k)
					{
						if (newTerms[i][k] != terms[j][k])
						{
							++diff;
							if(newTerms[i][k] != -1 && terms[j][k] != -1) diffDigit = k;
						}
					}
					if (diff == 1 && diffDigit != -1)
					{
						/// 两个项只有一位不同，合并（用-1表示）
						newTerms[i][diffDigit] = newTerms[j][diffDigit] = -1;
						merge = true;
					}
				}
			}
			if (!merge) break;
			terms = newTerms;
		}*/

		/// 合并相同项直到无法合并为止
		while (true)
		{
			bool merging = false; /// 根据是否进行了合并操作，标记是否仍在进行合并
			for (int i = 0; i < int(terms.size()); ++i)
			{
				for (int j = i + 1; j < int(terms.size()); ++j)
				{
					int diff = 0;
					for (int k = 0; k < variableCnt; ++k)
					{
						diff += terms[i][k] != terms[j][k];
					}
					if (diff == 0)
					{
						/// 两个项相同，将terms[j]合并到terms[i]，用相同方法维护coverage向量
						terms.erase(terms.begin() + j);
						coverage.erase(coverage.begin() + j);
						--j;

						/// 更新标记
						merging = true;
					}
				}
			}
			if (!merging) break;
		}

		/// 根据覆盖极小项的情况，进一步去掉多余项
		vector<vector<bool>> table;
		vector<int> termIndex; /// 每一行对应的项编号
		/// 构建表格
		for (int i = 0; i < int(terms.size()); ++i)
		{
			vector<bool> vec(1 << variableCnt, false);
			int k = coverage[i].size();
			for (int minterm : coverage[i])
			{
				vec[minterm] = true;
			}
			table.push_back(vec);
			termIndex.push_back(i);
		}
		/// 保留只含1个标识的列，把该标识对应的行划掉
		set<int> linesToErase;
		for (int i = 0; i < (1 << variableCnt); ++i)
		{
			int cnt = 0, line = -1;
			for (int j = 0; j < int(table.size()); ++j)
			{
				cnt += table[j][i];
				if (table[j][i]) line = j;
			}
			if (cnt == 1)
			{
				linesToErase.emplace(line);
			}
		}
		/*vector<int> linesToEraseVec;
		linesToEraseVec.assign(linesToErase.begin(), linesToErase.end());
		for (; !linesToEraseVec.empty(); linesToEraseVec.pop_back())
		{
			table.erase(table.begin() + *(linesToEraseVec.end() - 1));
			termIndex.erase(termIndex.begin() + *(linesToEraseVec.end() - 1));
		}*/
		/// 枚举法删项
		set<int> totalCoverage;
		for (set<int> termCoverage : coverage)
		{
			totalCoverage.insert(termCoverage.begin(), termCoverage.end());
		}
		while (true)
		{
			bool erasing = false; /// 根据是否进行了删项，标记是否仍在进行删项
			for (int i = 0; i < int(table.size()); ++i)
			{
				set<int> newCoverage_i; /// 去掉第i项时的覆盖情况
				for (int j = 0; j < int(table.size()); ++j)
				{
					if (j == i) continue;
					newCoverage_i.insert(coverage[termIndex[j]].begin(), coverage[termIndex[j]].end());
				}
				if (totalCoverage.size() == newCoverage_i.size())
				{
					/// 删项
					terms.erase(terms.begin() + termIndex[i]);

					/// 删行
					table.erase(table.begin() + i);
					termIndex.erase(termIndex.begin() + i);

					/// 更新标记
					erasing = true;
				}
			}
			if (!erasing) break;
		}

		/// 转化为命题公式并返回
		shared_ptr<Function> ret(make_shared<Function>());
		int tCnt = 0; /// 记录表达式所含项数
		for (int i = 0; i < int(terms.size()); ++i)
		{
			vector<int> vec = terms[i];
			shared_ptr<Function> term(make_shared<Function>());
			int vCnt = 0; /// 记录当前项所含变量个数
			for (int j = 0; j < variableCnt; ++j)
			{
				if (vec[variableCnt - j - 1] == -1) continue; /// 已合并的位

				shared_ptr<Function> v(make_shared<VariableAsFunction>(function->variables[j]));
				if (!vec[variableCnt - j - 1])
				{
					shared_ptr<Function> vn(make_shared<Negation>(v));
					v = vn;
				}
				if (vCnt == 0) term = v;
				else
				{
					term = make_shared<Conjunction>(term, v);
				}
				++vCnt;
			}
			
			if (vCnt == 0) continue; /// 空项
			if (tCnt == 0) ret = term;
			else
			{
				ret = make_shared<Disjunction>(ret, term);
			}
			++tCnt;
		}
		return ret;
	}
};
