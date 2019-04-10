#ifndef _CUSKETCH_H
#define _CUSKETCH_H

#include "paras.h"
#include "BOBHash32.h"
#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;

template <int memory_in_bytes, int d>
class CUSketch
{
private:
	static constexpr int w = memory_in_bytes * 8 / BITS_PER_BUCKET;
	BOBHash32 * hash[d];
public:
	static int counters[MAX_ARRAY_LEN];
	CUSketch()
	{
		memset(counters, 0, MAX_ARRAY_LEN * sizeof(int));
		for (int i = 0; i < d; i++)
			hash[i] = new BOBHash32(i + 750);
	}

	void print_basic_info()
	{
		printf("CU sketch\n");
		printf("\tCounters: %d\n", w);
		printf("\tMemory: %.6lfMB\n", w * 4.0 / 1024 / 1024);
	}

	~CUSketch()
	{
		for (int i = 0; i < d; i++)
			delete hash[i];
	}

	void insert(string key, int f)
	{
		for (int i = 0; i < f; i++) {
			int index = 0;
			int ret = INT_MAX;
			for (int j = 0; j < d; j++) {
				int tmp_index = (hash[j]->run(key.c_str(), KEY_LEN)) % w;
				if (counters[tmp_index] < ret) {
					ret = counters[tmp_index];
					index = tmp_index;
				}
			}
			int tmp_value = counters[index];

			for (int j = 0; j < d; j++) {
				int tmp_index = (hash[j]->run(key.c_str(), KEY_LEN)) % w;
				
				if (counters[tmp_index] == tmp_value) {
					counters[tmp_index] ++;
					if (counters[tmp_index] >= (1 << BITS_PER_BUCKET)) {
						counters[tmp_index] = (1 << BITS_PER_BUCKET) - 1;
					}
				}
			}

		}
	}

	int query(string key)
	{
		int ret = INT_MAX;
		for (int i = 0; i < d; i++) {
			int tmp = counters[(hash[i]->run(key.c_str(), KEY_LEN)) % w];
			ret = min(ret, tmp);
		}
		return ret;
	}

	void exp_res(vector<string> &data, unordered_map<string, uint32_t>& ground, vector<double>& result) {
		for (auto key : data) {
			insert(key, 1);
		}

		double aae = 0;
		double are = 0;
		for (auto key : ground) {
			int q_ret = query(key.first);
			int true_ret = ground[key.first];
			if (q_ret < true_ret) {
				cerr << "CU SKETCH ERROR, press Enter." << endl;
				cin.get();
				exit(-1);
			}
			aae = aae + q_ret - true_ret;
			are = are + (q_ret - true_ret) / true_ret;
		}
		aae = aae / ground.size();
		are = are / ground.size();
		result.clear();
		result.push_back(aae); result.push_back(are);

		cout << "CU: AAE:" << aae << ", ARE:" << are << endl;
	}

};

template <int memory_in_bytes, int d>
int CUSketch<memory_in_bytes, d>::counters[MAX_ARRAY_LEN];

#endif //_CUSKETCH_H