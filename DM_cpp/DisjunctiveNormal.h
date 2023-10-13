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

		wstring ret = L"��m(";
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

	shared_ptr<Function> simplify() /// ��������ȡ��ʽ����
	{
		if (isConstant) return make_shared<ConstantAsFunction>(constantValue);
		
		int variableCnt = function->variables.size();

		/// ��ȡ��ȡ��ʽ��������
		vector<vector<int>> terms;
		vector<set<int>> coverage; /// ��¼ÿ����ǵļ�С��
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

		/// �ϲ�������ֱ���޷��ϲ�Ϊֹ
		while (true)
		{
			bool merging = false; /// �����Ƿ�����˺ϲ�����������Ƿ����ڽ��кϲ�
			vector<vector<int>> newTerms;
			vector<set<int>> newCoverage;
			vector<bool> merged(terms.size(), false); /// ���ÿ�����Ƿ񱻺ϲ���
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
						/// ������ֻ��һλ��ͬ���ϲ�����-1��ʾ��
						vector<int> mergedTerm = terms[i];
						mergedTerm[diffDigit] = -1;
						newTerms.push_back(mergedTerm);

						/// �ϲ����������ǵļ�С���
						set<int> mergedCoverage = coverage[i];
						mergedCoverage.insert(coverage[j].begin(), coverage[j].end());
						newCoverage.push_back(mergedCoverage);

						/// ���±��
						merging = true;
						merged[i] = merged[j] = true;
					}
				}
			}

			if (!merging) break;
			/// ��δ�ϲ�����ԭ�ⲻ�����䵽newTerms��newCoverage
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
			bool merge = false; /// ����Ƿ�����˺ϲ�����
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
						/// ������ֻ��һλ��ͬ���ϲ�����-1��ʾ��
						newTerms[i][diffDigit] = newTerms[j][diffDigit] = -1;
						merge = true;
					}
				}
			}
			if (!merge) break;
			terms = newTerms;
		}*/

		/// �ϲ���ͬ��ֱ���޷��ϲ�Ϊֹ
		while (true)
		{
			bool merging = false; /// �����Ƿ�����˺ϲ�����������Ƿ����ڽ��кϲ�
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
						/// ��������ͬ����terms[j]�ϲ���terms[i]������ͬ����ά��coverage����
						terms.erase(terms.begin() + j);
						coverage.erase(coverage.begin() + j);
						--j;

						/// ���±��
						merging = true;
					}
				}
			}
			if (!merging) break;
		}

		/// ���ݸ��Ǽ�С����������һ��ȥ��������
		vector<vector<bool>> table;
		vector<int> termIndex; /// ÿһ�ж�Ӧ������
		/// �������
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
		/// ����ֻ��1����ʶ���У��Ѹñ�ʶ��Ӧ���л���
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
		/// ö�ٷ�ɾ��
		set<int> totalCoverage;
		for (set<int> termCoverage : coverage)
		{
			totalCoverage.insert(termCoverage.begin(), termCoverage.end());
		}
		while (true)
		{
			bool erasing = false; /// �����Ƿ������ɾ�����Ƿ����ڽ���ɾ��
			for (int i = 0; i < int(table.size()); ++i)
			{
				set<int> newCoverage_i; /// ȥ����i��ʱ�ĸ������
				for (int j = 0; j < int(table.size()); ++j)
				{
					if (j == i) continue;
					newCoverage_i.insert(coverage[termIndex[j]].begin(), coverage[termIndex[j]].end());
				}
				if (totalCoverage.size() == newCoverage_i.size())
				{
					/// ɾ��
					terms.erase(terms.begin() + termIndex[i]);

					/// ɾ��
					table.erase(table.begin() + i);
					termIndex.erase(termIndex.begin() + i);

					/// ���±��
					erasing = true;
				}
			}
			if (!erasing) break;
		}

		/// ת��Ϊ���⹫ʽ������
		shared_ptr<Function> ret(make_shared<Function>());
		int tCnt = 0; /// ��¼���ʽ��������
		for (int i = 0; i < int(terms.size()); ++i)
		{
			vector<int> vec = terms[i];
			shared_ptr<Function> term(make_shared<Function>());
			int vCnt = 0; /// ��¼��ǰ��������������
			for (int j = 0; j < variableCnt; ++j)
			{
				if (vec[variableCnt - j - 1] == -1) continue; /// �Ѻϲ���λ

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
			
			if (vCnt == 0) continue; /// ����
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
